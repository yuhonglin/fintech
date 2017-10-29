#ifndef ALG_H
#define ALG_H

#include <limits>
#include <iostream>
#include <cmath>
#include <vector>

#include "array.hpp"
#include "profile.hpp"
#include "funcarry.hpp"
#include "funcprof.hpp"
#include "model.hpp"

struct alg {
  
  func_prof func_state;
  func_prof func_action;

  int m;
  int n;

  std::vector<int> min_b_idx;
    
  model config;

  std::vector<double> R;
    
  alg(const model& mod) : func_state(mod.func_state),
			  func_action(mod.func_action),
			  n(mod.num_agent),
			  m(mod.num_normal),
			  config(mod) {
    init_R();
  };

  void init_R() {
    if (m!=4 or n!=2) {
      std::cerr<< "only support m=4 and n=2" << std::endl;
      exit(1);
    }
    R = { -1,0,
	  1,0,
	  0,1,
	  0,-1 };
    min_b_idx = {0, 3}; // MAYBE WRONG
  };
  
  // set the first part of A matrix of constraint
  void set_A_1(std::vector<double>& A) {
    // The first part is just the R matrix
    for (int i=0; i<R.size(); i++) {
      A[i] = R[i];
    }
  };

  // the second part of A
  void set_A_2(std::vector<double>& A) {
    // The second part of A is even easier
    int idx = R.size();
    for (int i=0; i<n; i++) { // for each agent
      A[idx] = -config.beta[i];
      idx += n+1;
    }
  };

  // set the first part of b
  void set_b_1(std::vector<double>& b, const profile& state_prof,
	       const profile& action_prof, const std::vector<double>& W) {
    // figure out the next state profile
    profile next_state = config.get_next_state(state_prof, action_prof);
    for (int i=0; i<m; i++) {
      b[i] = W[next_state.index()*m+i];
    }
  }

  // set the second part of b
  void set_b_2(std::vector<double>& b, const profile& state_prof,
	       const profile& action_prof, const std::vector<double>& W) {
    // get current profit
    std::vector<double> crnt_profit = config.get_profit(state_prof, action_prof);
	
    profile iter;
    for (int i=0; i<n; i++) { // for each agent
      iter = action_prof;
      func_action.reset(iter, i);
      double minvalue = std::numeric_limits<double>::max();
      for ( ; iter!=func_action.end(); func_action.inc_only(iter,i) ) {
	std::vector<double> iter_profit = config.get_profit(state_prof, iter);
	profile iter_nxtstat = config.get_next_state(state_prof, iter);
	double min_b = W[iter_nxtstat.index()*m + min_b_idx[i]];
	double tmp = (1-config.beta[i])*(crnt_profit[i]-iter_profit[i]) - config.beta[i]*min_b;
	if (minvalue > tmp) {
	  tmp = minvalue;
	}
      }
      b[i] = minvalue;
    }
  };

  void linprog(const int&n, const int&m, const double* r,
	       const double* A, const double* b,
	       double& f, int &status) {
    if (m!=4 || n!=2) {
      std::cerr<< "only support m=4 and n=2" << std::endl;
      exit(1);
    }
    // check feasibility
    if (b[1] < b[4]/A[8] || b[2] < b[5]/A[10]) {
      status = -1;
      f = std::numeric_limits<double>::min();
      return;
    }
    
    f = 0;
    for (int i=0; i<n; i++) {
      if (r[i] > 0) {
	f += r[i]*b[i+1]; // b's upper bound
      } else {
	f += r[i]*std::max(-b[3*i], b[m+i]/A[R.size()+i*n+i]); // b's lower bound
      }
    }
  };
  
  void solve() {
    std::vector<double> W(func_state.card()*m, 5);
    std::vector<double> W_new(W);

    profile action_prof = func_action.begin();
    profile state_prof = func_state.begin();
	
    while (true) {
      for ( ; state_prof != func_state.end(); func_state.inc(state_prof)) {
	for (int i = 0; i<m ; i++) {
	  // For each state and normal, we need to update
	  // the correponding constant and store it to
	  // wks, a per-thread vector to avoid false sharing
	  std::vector<double> wks(func_action.card());
	  // the constraints
	  std::vector<double> A((n + m)*n, 0.);
	  std::vector<double> b(n + m);
	  // set the first part of A
	  set_A_1(A);
	  // set the second part of A
	  set_A_2(A);

	  int idx = 0;
	  for ( ; action_prof != func_action.end(); func_action.inc(action_prof)) {
	    // generate first part of b
	    set_b_1(b, state_prof, action_prof, W);
	    // generate second part of b
	    set_b_2(b, state_prof, action_prof, W);
			
	    // do the linear programming
	    double f;
	    int status;
	    linprog(n, m, R.data()+i*n, A.data(), b.data(), f, status);
	    
	    // store optimal value to wks
	    wks[i] = f;
	  }
	  // find the maximum of wks
	  W_new[state_prof.index()*m+i] = *std::max(wks.begin(), wks.end());
	} // for i
      }  // for state_prof

      // test convergence
      double normdiff = 0;
      for (int convidx = 0; convidx < W.size(); convidx++) {
	normdiff += std::pow(W[convidx] - W_new[convidx],2);
      }
      normdiff /= W.size();

      if (normdiff < 0.1) {
	// converged
      } else {
	W = W_new;
	continue;
      }
    }
  };
};


#endif /* ALG_H */
