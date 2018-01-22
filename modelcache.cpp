#include <limits>

#include "modelcache.hpp"

ModelCache::ModelCache(model* mod) : config(mod), func_state(mod->get_state_func()) {};



#ifdef USE_MODEL_CACHE
profile& ModelCache::get_next_state(const profile& sp, const profile& ap) {
  return next_state[sp.index()][ap.index()];
}
#else
profile ModelCache::get_next_state(const profile& sp, const profile& ap) {
  auto ret = config->get_next_state(sp, ap);
  func_state.update_index(ret);
  return ret;
}  
#endif


#ifdef USE_MODEL_CACHE
double* ModelCache::get_profit(const profile& sp, const profile& ap) {
  return profit[sp.index()].data() + ap.index()*config->num_agent();
}
#else
std::vector<double> ModelCache::get_profit(const profile& sp, const profile& ap) {
  return config->get_profit(sp, ap);
}
#endif

void ModelCache::build() {
#ifdef USE_MODEL_CACHE
  // empty and reserve the cache
  profit.clear(); profit.reserve(func_state.card());
  next_state.clear(); next_state.reserve(func_state.card());

  stage_profit_bound_.first.clear(); stage_profit_bound_.first.reserve(config->num_agent());
  stage_profit_bound_.second.clear(); stage_profit_bound_.second.reserve(config->num_agent());
  for (int i = 0; i < config->num_agent(); i++) {
    stage_profit_bound_.first.push_back(-std::numeric_limits<double>::infinity());
    stage_profit_bound_.second.push_back(std::numeric_limits<double>::infinity());
  }

  if_skip_action_prof.clear(); if_skip_action_prof.reserve(func_state.card());
  
  // build
  for (profile sp = func_state.begin(); sp != func_state.end(); func_state.inc(sp)) {
    func_prof func_action = config->get_action_func(sp);
    profit.push_back({}); profit.back().reserve(config->num_agent()*func_action.card());
    next_state.push_back({}); next_state.back().reserve(func_action.card());
    if_skip_action_prof.push_back({}); if_skip_action_prof.back().reserve(func_action.card());
					 
    for (profile ap = func_action.begin(); ap != func_state.end(); func_action.inc(ap)) {
      auto prft = config->get_profit(sp, ap);
      profit.back().insert(profit.back().end(), prft.begin(), prft.end());

      for (int i = 0; i < config->num_agent(); i++) {
	if (prft[i] > stage_profit_bound_.first[i])  stage_profit_bound_.first[i] = prft[i];
	if (prft[i] < stage_profit_bound_.second[i]) stage_profit_bound_.second[i] = prft[i];
      }
      
      auto nxtst = config->get_next_state(sp, ap);
      func_state.round(nxtst);
      next_state.back().push_back(nxtst);

      if_skip_action_prof.back().push_back(config->if_skip(sp, ap)); 
    }
  }

  std::cout << "Maximum stage payoffs: ";
  for (auto &i : stage_profit_bound_.first) std::cout << i << '\t';
  std::cout << '\n';
  std::cout << "Minimum stage payoffs: ";
  for (auto &i : stage_profit_bound_.second) std::cout << i << '\t';
  
#endif
}

#ifdef USE_MODEL_CACHE
std::pair<std::vector<double>, std::vector<double>>
ModelCache::stage_profit_bound() {
  return stage_profit_bound_;
}
#else
std::pair<std::vector<double>, std::vector<double>>
ModelCache::stage_profit_bound() {
  return config->stage_profit_bound();
}
#endif


#ifdef USE_MODEL_CACHE
bool ModelCache::if_skip(const profile& sp, const profile &ap) const {
  if_skip_action_prof[sp.index()][ap.index()];
}
#else
bool ModelCache::if_skip(profile &ap) const {
  return config->if_skip(ap);
}
#endif
