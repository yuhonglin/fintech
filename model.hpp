#ifndef MODEL_H
#define MODEL_H

#include <numeric>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include "array.hpp"
#include "profile.hpp"
#include "funcarry.hpp"
#include "funcprof.hpp"


struct model {
  func_prof func_state;
  func_prof func_action;

  int num_agent;
  int num_normal;

    double init_w;
  
  // discount factor for each firm;
  std::vector<double> beta;

  //////////////
  // iterface //
  //////////////
  
  // law of motion
  profile get_next_state(profile& s, profile& a);

  // profit function
  std::vector<double> get_profit(profile& s, profile& a);

  /////////////////////
  // helper function //
  /////////////////////

  
  double F(const double &theta);
  
  std::vector<double> get_quantity(profile& a);

  std::vector<double> get_setup_cost(profile &s, profile &a);

  std::vector<double> get_unitproduct_cost(profile &a);

  std::vector<double> get_unitclientcapital_cost(profile &a);

  std::vector<double> get_clientcapital_incfactor(const profile &a);
  
  std::vector<double> get_clientcapital_decfactor(const profile &s, const profile &a);
  
};


#endif /* MODEL_H */
