#ifndef MODELCACHE_H
#define MODELCACHE_H

#include <iostream>
#include <vector>

#include "array.hpp"
#include "profile.hpp"
#include "funcarry.hpp"
#include "funcprof.hpp"
#include "model.hpp"


// A class to cache the model, including
//  - current profit based on state and action
//  - next profit based on state and action
//  - next state based on current state and action
// The movitation of this caching is that, the model
// actually has nothing to do with W/W_new.
class ModelCache
{
  func_prof func_state;
  model* config;
  
public:
  ModelCache(model* mod);
  virtual ~ModelCache() = default;

  // std::vector<std::vector<double>> crnt_profit;
  // std::vector<std::vector<double>> next_profit;
  std::vector<std::vector<double>> profit;
  std::vector<std::vector<profile>> next_state;

  profile& get_next_state(profile& sp, profile& ap);

  double* get_profit(profile& sp, profile& ap);

  void build();
  
};


#endif /* MODELCACHE_H */
