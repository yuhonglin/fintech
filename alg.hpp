#ifndef ALG_H
#define ALG_H

#include <limits>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <memory>
#include <map>

#include "modelcache.hpp"
#include "array.hpp"
#include "profile.hpp"
#include "funcarry.hpp"
#include "funcprof.hpp"
#include "model.hpp"
#include "lpsolver.hpp"


struct alg {
  const double eps = 1e-12;
  
  int m;
  int n;

  std::vector<int> min_b_idx;

  double init_w;

  std::vector<double> R;
  
  model* config; // observer
  ModelCache cache;

  std::string output_dir;
  
  alg(model* mod);

  void init_R();

  int num_thread_;
  
  // set the first part of ub
  void set_lb(std::vector<double>& b, func_prof& func_state, func_prof& func_action, profile& state_prof,
	       profile& action_prof, std::vector<double>& W);

  // set the second part of ub
  void set_ub(std::vector<double>& b, func_prof& func_state, func_prof& func_action, profile& state_prof,
	       profile& action_prof, std::vector<double>& W);

  void solve();

  void save_grad();
  void save_stat();

  void init_W(std::vector<double>& W);
};


#endif /* ALG_H */
