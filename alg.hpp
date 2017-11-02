#ifndef ALG_H
#define ALG_H

#include <limits>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>

#include "array.hpp"
#include "profile.hpp"
#include "funcarry.hpp"
#include "funcprof.hpp"
#include "model.hpp"
#include "lpsolver.hpp"

struct alg {
  
  func_prof func_state;
  func_prof func_action;

  int m;
  int n;

  std::vector<int> min_b_idx;
    
  model config;

  std::vector<double> R;

  double init_w;
  alg(model& mod);

  void init_R();
  
  // set the first part of A matrix of constraint
  void set_A_1(std::vector<double>& A);

  // the second part of A
  void set_A_2(std::vector<double>& A);

  // set the first part of ub
  void set_lb(std::vector<double>& b, profile& state_prof,
	       profile& action_prof, std::vector<double>& W);

  // set the second part of ub
  void set_ub(std::vector<double>& b, profile& state_prof,
	       profile& action_prof, std::vector<double>& W);

  void solve();
};


#endif /* ALG_H */
