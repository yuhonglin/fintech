#ifndef MODELCACHE_H
#define MODELCACHE_H

#define USE_MODEL_CACHE

#include <iostream>
#include <vector>
#include <utility>

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

#ifdef USE_MODEL_CACHE  
  std::pair<std::vector<double>, std::vector<double>>
  stage_profit_bound_;
#endif
  
public:
  ModelCache(model* mod);
  virtual ~ModelCache() = default;

  // std::vector<std::vector<double>> crnt_profit;
  // std::vector<std::vector<double>> next_profit;
  std::vector<std::vector<double>> profit;
  std::vector<std::vector<profile>> next_state;

#ifdef USE_MODEL_CACHE
  profile& get_next_state(profile& sp, profile& ap);
#else
  profile get_next_state(profile& sp, profile& ap);
#endif

#ifdef USE_MODEL_CACHE
  double* get_profit(profile& sp, profile& ap);
#else
  std::vector<double> get_profit(profile& sp, profile& ap);
#endif
  
  void build();
  
  std::pair<std::vector<double>, std::vector<double>>
  stage_profit_bound();
  
};


#endif /* MODELCACHE_H */
