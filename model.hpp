#ifndef MODEL_H
#define MODEL_H

#include <numeric>
#include <iostream>
#include <cmath>
#include <vector>

#include "array.hpp"
#include "profile.hpp"
#include "funcarry.hpp"
#include "funcprof.hpp"


struct model {
  func_prof func_state;
  func_prof func_action;

  int num_agent;
  int num_normal;
  
  // discount factor for each firm;
  std::vector<double> beta;

  // law of motion
  profile get_next_state(const profile& p, const profile& a) {
    const double d1 = 0;
    return profile();
  }

  // profit function
  std::vector<double> get_profit(const profile& p, const profile& a) {
    return std::vector<double>();
  }
};


#endif /* MODEL_H */
