#include <limits>
#include <sstream>
#include <cmath>

#include <omp.h>

#include "alg.hpp"

#include "FinTech.hpp"

#define USE_FORTRAN_SOVLER

void debug_print(double* d, int nrow, int ncol) {
  for (int i=0; i<nrow; i++) {
    for (int j=0; j<ncol; j++) {
#ifdef USE_FORTRAN_SOVLER
      std::cout << d[i+j*nrow] << '\t';
#else
      std::cout << d[i*ncol+j] << '\t';
#endif      
    }
    std::cout << '\n';
  }
}


alg::alg(model* mod) : n(mod->num_agent()),
		       init_w(mod->init_constant()),
		       config(mod) {
  if (mod->num_normal() % 4 != 0) {
    m = std::round(mod->num_normal()/4)*4;
    std::cout << "warning: round number of normals to " << m << std::endl;
  }

  m = mod->num_normal();
  
  init_R();

  num_thread_ = 20;
};


void alg::init_R() {
  if (n!=2) {
    std::cerr<< "only support n=2" << std::endl;
    exit(1);
  }
  R.reserve(m*2);
  const double pi = 3.14159265358979323846264338;
  double tmp;
  for (int i=0; i<m; i++) {
    tmp = std::cos(i*2*pi/m);
    if (std::abs(tmp) < 1e-15)
      R.push_back(0.);
    else
      R.push_back(tmp);

    tmp = std::sin(i*2*pi/m);
    if (std::abs(tmp) < 1e-15)
      R.push_back(0);
    else
      R.push_back(tmp);
  }

#ifdef USE_FORTRAN_SOVLER
  std::vector<double> R_trans(R.size());
  for (int i=0; i<m; i++)
    for (int j=0; j<n; j++) {
      R_trans[j*m+i] = R[i*n+j];
    }
  R = R_trans;
#endif

  min_b_idx = {m/2, m/4*3}; // MAYBE WRONG
  
}

// set the lower bound
void alg::set_lb(std::vector<double>& b, func_prof& func_state, func_prof& func_action, profile& state_prof,
		  profile& action_prof, std::vector<double>& W) {
  // get current profit
  std::vector<double> crnt_profit = config->get_profit(state_prof, action_prof);

  profile iter;
  // CHECK: whether newly compute or use the previous one in the calling loop?
  // func_prof func_action = mod.get_action_func(state_prof);
  for (int i=0; i<n; i++) { // for each agent
    iter = action_prof;
    func_action.reset(iter, i);
    double maxvalue = -std::numeric_limits<double>::infinity();
    for ( ; iter!=func_action.end(); func_action.inc_only(iter,i) ) {
      if (iter == action_prof)
	continue;
      std::vector<double> iter_profit = config->get_profit(state_prof, iter);
      
      profile iter_nxtstat = config->get_next_state(state_prof, iter);       // Do NOT forget to round it!!!
      func_state.round(iter_nxtstat);

      double min_b = -W[iter_nxtstat.index()*m + min_b_idx[i]];
      double tmp = ( (1-config->beta()[i])*(iter_profit[i]-crnt_profit[i])
		     + config->beta()[i]*min_b ) / config->beta()[i];

      //      if (tmp > W[state_prof.index()*m + min_b_idx[i]]) {
      if (false) {      
	std::cout << i << std::endl;
	// std::cout << iter_profit[i] << '\t' << crnt_profit[i] << std::endl;
	std::cout << "current  state: " << state_prof       << std::endl;
	std::cout << "   iter  state: " << iter_nxtstat     << std::endl;
	std::cout << "current profit: " << crnt_profit[i]   << std::endl;
	std::cout << "   iter profit: " << iter_profit[i]   << std::endl;
	std::cout << "current action: " << action_prof      << std::endl;
	std::cout << "   iter action: " << iter             << std::endl;

	std::cout << "current setup cost: " << dynamic_cast<FinTech*>(config)->get_setup_cost(state_prof, action_prof)[i]  << std::endl;
	std::cout << "   iter setup cost: " << dynamic_cast<FinTech*>(config)->get_setup_cost(state_prof, iter)[i]  << std::endl;
	std::cout << "  current quantity: " << dynamic_cast<FinTech*>(config)->get_quantity(action_prof)[i]  << std::endl;
	std::cout << "     iter quantity: " << dynamic_cast<FinTech*>(config)->get_quantity(iter)[i]  << std::endl;
	std::cout << " current unit cost: " << dynamic_cast<FinTech*>(config)->get_unitproduct_cost(action_prof)[i]  << std::endl;
	std::cout << "    iter unit cost: " << dynamic_cast<FinTech*>(config)->get_unitproduct_cost(iter)[i]  << std::endl;

	// std::cout << action_prof << '\n' << iter << std::endl;
	std::cout << "tmp: " << tmp << std::endl;
	std::cin.get();
      }


      if (maxvalue < tmp) {
	maxvalue = tmp;
      }
    }
    b[i] = maxvalue;
  }
  for (int i=n; i<n+m; i++) {
    b[i] = -std::numeric_limits<double>::infinity();
  }
}

// set upper bound
void alg::set_ub(std::vector<double>& b, func_prof& func_state, func_prof& func_action, profile& state_prof,
		 profile& action_prof, std::vector<double>& W) {
  // figure out the next state profile
  profile next_state = config->get_next_state(state_prof, action_prof); // // Do NOT forget to round it!!!
  func_state.round(next_state);
  
  for (int i=n; i<n+m; i++) {
    b[i] = W[next_state.index()*m+i-n];
  }

  for (int i=0; i<n; i++) {
    b[i] = std::numeric_limits<double>::infinity();
  }
};

void alg::save_grad() {
  // store sub-gradients
  std::stringstream ss;
  ss << output_dir << "/gradient";
  std::ofstream of(ss.str());
  for (int i=0; i < m; i++) {
    for (int j=0; j < n; j++) {
#ifdef USE_FORTRAN_SOVLER
      of << R[j*m+i] << '\t';
#else
      of << R[i*n+j] << '\t';
#endif
    }
    of << '\n';
  }
  of.close();
}


void alg::solve() {

  // first, save the sub-gradients
  save_grad();
  
  // Get the functor of state profiles, used in looping
  auto func_state = config->get_state_func();

  // store the (working) constants
  std::vector<double> W(func_state.card()*m, init_w);
  std::vector<double> W_new(W);

  int loop_index = 0;
  while (true) {

    auto sp_all = func_state.get_all();

#pragma omp parallel for num_threads(num_thread_)    
    for (int spidx = 0; spidx < func_state.card(); spidx++) {
      profile state_prof = sp_all[spidx];
      // report progress
      if (state_prof.index()%10 == 0) {
	std::cout << "progress : " << float(state_prof.index())/func_state.card() << std::endl;
      }
      
      // get the functor of actions, used for iteration
      func_prof func_action = config->get_action_func(state_prof);
      
      for (int i = 0; i<m ; i++) {
	// allocate the solver, EACH PER THREAD
	lp_solver linprog(n,m);
	std::vector<double> c(n,0);   // objective
	std::vector<double> x(n,0);   // solution
	
	// For each state and normal, we need to update
	// the correponding constant and store it to
	// wks, a per-thread vector to avoid false sharing
	std::vector<double> wks(func_action.card());
	
	// the constraints
	std::vector<double> A(m*n, 0.);
	std::vector<double> lb(n+m, 0.);
	std::vector<double> ub(n+m, 0.);
	
	auto iter_wks = wks.begin();
	for ( profile action_prof  = func_action.begin();
	              action_prof != func_action.end();
	              func_action.inc(action_prof) ) {
	  // set the lower bounds of the constraints
	  set_lb(lb, func_state, func_action, state_prof, action_prof, W);
	  // set the upper bounds of the constraints
	  set_ub(ub, func_state, func_action, state_prof, action_prof, W);
	  
	  // do the linear programming
	  double f;
	  lp_solver::STATUS status;

	  // process objective
	  double *r = R.data();
	  
#ifdef USE_FORTRAN_SOVLER	  
	  for (int j=0; j<n; j++) c[j] = -config->beta()[j]*r[j*m+i];
#else
	  for (int j=0; j<n; j++) c[j] = -config->beta()[j]*r[j+i*n];
#endif
	  
	  linprog.solve(c.data(), R.data(), lb.data(), ub.data(), 0, f, x.data(), status);

	  if (status == lp_solver::ERROR) {
	    std::cerr << "meet error in optimisation" << std::endl;
	    exit(1);
	  }

	  // store optimal value to wks
	  if (status == lp_solver::INFEASIBLE) {
	    std::cout << "[Infeasible]" << std::endl;
	    debug_print(R.data(), m, n);
	    for (int pidx = 0; pidx < n+m; pidx++) {
	      std::cout << lb[pidx] << '\t' << ub[pidx] << std::endl;
	    }
	    (*iter_wks) = -std::numeric_limits<double>::infinity();
	  } else {
	    (*iter_wks) = -f;
	  }
	  std::vector<double> crnt_profit = config->get_profit(state_prof, action_prof);
	  for (int j=0; j<n; j++) {
#ifdef USE_FORTRAN_SOVLER
	    (*iter_wks) += (1-config->beta()[j])*r[j*m+i]*crnt_profit[j];
#else	    
	    (*iter_wks) += (1-config->beta()[j])*r[j+i*n]*crnt_profit[j];
#endif
	  }
	  iter_wks++;
	  
	} // for action_prof
	
	// find the maximum of wks
	W_new[state_prof.index()*m+i] =
	  (*std::max_element(wks.begin(), wks.end()));
	
      } // for i (normals)
      
    }  // for state_prof

    // test convergence
    double normdiff = 0;
    for (int convidx = 0; convidx < W.size(); convidx++) {
      normdiff += std::abs(W[convidx] - W_new[convidx]);
    }
    normdiff /= W.size();

    std::cout << "difference: " <<  normdiff << std::endl;

    // store W_new
    loop_index++;
    std::stringstream ss;
    ss << output_dir << "/data_" << loop_index;
    std::ofstream of(ss.str());
    int widx = 0;
    for (int si =0; si < func_state.card(); si++) {
      for (int sj=0; sj < m; sj++) {
	of << W_new[widx] << '\t';
	widx++;
      }
      of << '\n';
    }
    of.close();
	  
    if (normdiff < 0.000001) {
      // converged
      break;
    } else {
      W = W_new;
      continue;
    }
  } // while()

} // solve()
