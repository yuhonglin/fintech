#include <limits>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <algorithm>

#include <omp.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>

#include "util.hpp"

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
		       config(mod),
		       cache(mod) {
  if (mod->num_normal() % 4 != 0) {
    m = std::round(mod->num_normal()/4)*4;
    std::cout << "warning: round number of normals to " << m << std::endl;
  }

  m = mod->num_normal();
  
  init_R();

  util::tic("Building model cache");
  cache.build();
  util::toc("Building model cache: done");

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
void alg::set_lb(std::vector<double>& b, func_prof& func_state,
		 func_prof& func_action, profile& state_prof,
		 profile& action_prof, std::vector<double>& W) {
  // get current profit
  auto crnt_profit = cache.get_profit(state_prof, action_prof);

  profile iter;
  // CHECK: whether newly compute or use the previous one in the calling loop?
  // func_prof func_action = mod.get_action_func(state_prof);
  for (int i=0; i<n; i++) { // for each agent
    iter = action_prof;
    func_action.reset(iter, i);

    double maxvalue = -std::numeric_limits<double>::infinity();
    for ( ; iter!=func_action.end(); func_action.inc_only(iter,i) ) {      
      ////if (iter == action_prof)
      ////continue;
      auto iter_profit = cache.get_profit(state_prof, iter);
      
      auto iter_nxtstat = cache.get_next_state(state_prof, iter);

      double min_b = -W[iter_nxtstat.index()*m + min_b_idx[i]];
      double tmp = ( (1-config->beta()[i])*(iter_profit[i]-crnt_profit[i])
      		     + config->beta()[i]*min_b ) / config->beta()[i];

      //      if (i==1)
      //      std::cout << iter << '\t' << tmp << std::endl;
      
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
void alg::set_ub(std::vector<double>& b, func_prof& func_state,
		 func_prof& func_action, profile& state_prof,
		 profile& action_prof, std::vector<double>& W) {
  // figure out the next state profile
  auto next_state = cache.get_next_state(state_prof, action_prof);
  
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

void alg::save_stat() {
  // store states and their index
  // store sub-gradients
  std::stringstream ss;
  ss << output_dir << "/state_profile";
  std::ofstream of(ss.str());
  
  auto func_state = config->get_state_func();
  for(profile i = func_state.begin(); i!=func_state.end(); func_state.inc(i)) {
    of << i << std::endl;
  }
  of.close();  
}

void alg::solve() {

  // Get the functor of state profiles, used in looping
  auto func_state = config->get_state_func();

  // loop index
  int loop_index = 0;

  // store the (working) constants
  std::vector<double> W(func_state.card()*m);
  init_W(W);
  
  std::vector<double> W_new(W);

  // check if to recover
  {
    std::stringstream ss;
    ss << output_dir << "/W_*";
    auto g = util::glob(ss.str());

    std::string cmd;
    if (g.size()!=0) {
      while(true) {
	std::cout << "Existing progress detected, continue [Y/n]?";
	std::getline(std::cin, cmd);
	if (cmd == "" or cmd == "y")  cmd = "Y";
	if (cmd == "n")               cmd = "N";
	if (cmd != "Y" and cmd != "N")
	  std::cout << "Please input Y or N." << std::endl;
	else
	  break;
      }

      if (cmd == "Y") {
	std::string last_file;
	int last_index = -1;
	for (const auto& f : g) {
	  auto tmp = util::split(f, '.');
	  int idx = std::stoi(util::split(tmp[tmp.size()-2], '_')[1]);
	  if (idx > last_index) {
	    last_file = f;
	    last_index = idx;
	  }
	}
	std::cout << "loading the previous file : " << last_file << std::endl;
	std::ifstream is(last_file, std::ios::binary);
	cereal::BinaryInputArchive archive(is);
	archive(W);
	is.close();
	
	loop_index = last_index + 1;
      } else {
	save_grad();
	save_stat();
	loop_index = 0;
      }
    } else {
      	save_grad();
	save_stat();
    }
  }

  // store the continuation payoffs
  // value: continuation payoff vectors for normals 1 to m for each firm
  //        the vector can be viewed as a card(stateprof)xmxn tensor
  //        
  std::vector<double> cont_payoff(func_state.card()*m*n,
				  std::numeric_limits<double>::quiet_NaN());

  // store the current profit
  std::vector<double> crnt_payoff(func_state.card()*m*n,
				  std::numeric_limits<double>::quiet_NaN());
  
  // store the action profiles at equilibrium vertex
  // value: a card(stateprof)xm tensor
  std::vector<profile> equi_actprof(func_state.card()*m, n);

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
      
      // allocate the solver, EACH PER THREAD!!!
      lp_solver linprog(n,m);

      // For each state and normal, we need to update
      // the correponding constant and store it to
      // wks, a per-thread vector to avoid false sharing
      std::vector<std::vector<double>>  nml_wks;
      std::vector<std::vector<profile>> nml_eap;
      std::vector<std::vector<double>>  nml_crntpf;
      std::vector<std::vector<double>>  nml_contpf;

      std::vector<std::vector<double> ::iterator> nml_iterwks;
      std::vector<std::vector<profile>::iterator> nml_itereap;
      std::vector<std::vector<double> ::iterator> nml_itercrntpf;  
      std::vector<std::vector<double> ::iterator> nml_itercontpf;
      
      for (int tmpi = 0; tmpi<m ; tmpi++) {
	nml_wks.push_back( std::vector<double>(func_action.card(), 0.) );
	nml_eap.push_back( std::vector<profile>(func_action.card(), 0) );
	nml_crntpf.push_back( std::vector<double>(func_action.card()*n,
						  std::numeric_limits<double>::quiet_NaN()) );
	nml_contpf.push_back( std::vector<double>(func_action.card()*n,
						  std::numeric_limits<double>::quiet_NaN()) );

	nml_iterwks.push_back(nml_wks.back().begin());
	nml_itereap.push_back(nml_eap.back().begin());   
	nml_itercrntpf.push_back(nml_crntpf.back().begin());
	nml_itercontpf.push_back(nml_contpf.back().begin());
      }
      
      // This loop should be out-side-of the normal loop because the constraints
      // do not depend on normals, that is, the constraints do not change over different
      // normals.
      for ( profile action_prof  = func_action.begin();
	    action_prof != func_action.end();
	    func_action.inc(action_prof) ) {      

	// the constraints
	std::vector<double> lb(n+m, 0.);
	std::vector<double> ub(n+m, 0.);
	
	// set the lower bounds of the constraints
	set_lb(lb, func_state, func_action, state_prof, action_prof, W);
	// set the upper bounds of the constraints
	set_ub(ub, func_state, func_action, state_prof, action_prof, W);

	// do linear programming for each normal
	for (int i = 0; i<m ; i++) {

	  std::vector<double> c(n,0);   // objective
	  std::vector<double> x(n,0);   // solution

	  // pick up the previous iterators
	  auto& iter_wks    = nml_iterwks[i];
	  auto& iter_eap    = nml_itereap[i];
	  auto& iter_crntpf = nml_itercrntpf[i];
	  auto& iter_contpf = nml_itercontpf[i];

	  // do the linear programming
	  double f;
	  lp_solver::STATUS status;

	  // process objective: TODO: this is a constant, should not generate it everytime.
	  const double *r = R.data();
	  
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
	    (*iter_wks) = -std::numeric_limits<double>::infinity();
	  } else {
	    (*iter_wks) = -f;
	  }
	  auto crnt_profit = cache.get_profit(state_prof, action_prof); // TODO: this may be moved out of this loop
	  for (int j=0; j<n; j++) {
#ifdef USE_FORTRAN_SOVLER
	    (*iter_wks) += (1-config->beta()[j])*r[j*m+i]*crnt_profit[j];
#else	    
	    (*iter_wks) += (1-config->beta()[j])*r[j+i*n]*crnt_profit[j];
#endif
	    (*iter_crntpf) = crnt_profit[j]; iter_crntpf++;
	    (*iter_contpf) = x[j]; iter_contpf++;
	  }

	  iter_wks++;

	  (*iter_eap) = action_prof;
	  iter_eap++;

	} // for i (normals)
      } // for action_prof

      // figure out the maximums
      for (int i = 0; i < m; i++) {
	std::vector<double> & wks    = nml_wks[i];
	std::vector<profile>& eap    = nml_eap[i];
	std::vector<double> & crntpf = nml_crntpf[i];
	std::vector<double> & contpf = nml_contpf[i];
	
	// find the index of maximum value of wks
	int optidx = 0;
	double optwks = -std::numeric_limits<double>::infinity();
	for (int oi = 0; oi < wks.size(); oi++) {
	  // if (optwks < wks[oi]-eps) {
	  if (optwks < wks[oi]) {
	    optwks = wks[oi];
	    optidx = oi;
	  }
	}
	W_new[state_prof.index()*m+i] = optwks;
	equi_actprof[state_prof.index()*m+i] = eap[optidx];

	for (int k=0; k<n; k++) {
	  crnt_payoff[state_prof.index()*m*n + n*i + k] = crntpf[optidx*n + k];
	  cont_payoff[state_prof.index()*m*n + n*i + k] = contpf[optidx*n + k];
	}
      } // for i (normals)

    }  // for state_prof
    
    // test convergence
    double maxdiff = -1;
    for (int convidx = 0; convidx < W.size(); convidx++) {
      if (maxdiff < std::abs(W[convidx] - W_new[convidx]))
	maxdiff = std::abs(W[convidx] - W_new[convidx]);
    }

    std::cout << "maximum difference: " <<  maxdiff << std::endl;

    // store W_new
    {
      // store in plain text, for easily reading
      {
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
      }
      // store in cereal for progress recover
      {
    	std::stringstream ss;
    	ss << output_dir << "/W_" << loop_index << ".cereal";
    	std::ofstream of(ss.str());
    	cereal::BinaryOutputArchive archive(of);
    	archive(W_new);
    	of.close();
      }
    }
    // store continuation payoffs
    {
    	std::stringstream ss;
    	ss << output_dir << "/contpayoff_" << loop_index;
    	std::ofstream of(ss.str());
    	for (auto &k : cont_payoff)
    	    of << k << '\n';
    	of.close();
    }
    // store current payoffs
    {
    	std::stringstream ss;
    	ss << output_dir << "/currentoff_" << loop_index;
    	std::ofstream of(ss.str());
    	for (auto &k : crnt_payoff)
    	    of << k << '\n';
    	of.close();
    }


    // test if monotonic
    for (int i = 0; i < W.size(); i++) {
      if (W[i] < W_new[i] - eps) {
   	std::cout << "Error: not monotonic." << std::endl;
	std::cout << W[i] << '\t' << W_new[i] << '\t' << W[i] - W_new[i] << std::endl;
	//	exit(1);
      }
    }


    // store equilibrium action profiles
    {
    	std::stringstream ss;
    	ss << output_dir << "/equiactprof_" << loop_index;
    	std::ofstream of(ss.str());
    	for (auto &k : equi_actprof)
    	    of << k << '\n';
    	of.close();
    }
    
    if (maxdiff < 1e-12) {
      // converged
      break;
    } else {
      loop_index++;
      W = W_new;
      continue;
    }
  } // while()

} // solve()

void alg::init_W(std::vector<double>& W) {
  if (n!=2) {
    std::cerr<< "only support n=2" << std::endl;
    exit(1);
  }
  // The initalization method is:
  //  - init the W corresponding to upper and lower bounds based on spb.
  //  - init the other W to inf.
  auto spb = cache.stage_profit_bound();
  auto func_state = config->get_state_func();
  
  for (int i = 0; i < W.size(); i++) W[i] = std::numeric_limits<double>::infinity();
  
  for (auto state_prof = func_state.begin(); state_prof != func_state.end();
       func_state.inc(state_prof)) {
    // max profit for first player
    W[state_prof.index()*m + 0] = spb.first[0];
    // max profit for second player
    W[state_prof.index()*m + m/4] = spb.first[1];
    // min profit for first player
    W[state_prof.index()*m + m/2] = -spb.second[0];
    // min profit for second player
    W[state_prof.index()*m + m/4*3] = -spb.second[1];
  }
}
