#include "modelcache.hpp"

ModelCache::ModelCache(model* mod) : config(mod), func_state(mod->get_state_func()) {};


profile& ModelCache::get_next_state(profile& sp, profile& ap) {
  return next_state[sp.index()][ap.index()];
}

double* ModelCache::get_profit(profile& sp, profile& ap) {
  return profit[sp.index()].data() + ap.index()*config->num_agent();
}

void ModelCache::build() {
  // empty and reserve the cache
  profit.clear(); profit.reserve(func_state.card());
  next_state.clear(); next_state.reserve(func_state.card());

  // build
  for (profile sp = func_state.begin(); sp != func_state.end(); func_state.inc(sp)) {
    func_prof func_action = config->get_action_func(sp);
    profit.push_back({}); profit.back().reserve(config->num_agent()*func_action.card());
    next_state.push_back({}); next_state.back().reserve(func_action.card());
    for (profile ap = func_action.begin(); ap != func_state.end(); func_action.inc(ap)) {
      auto prft = config->get_profit(sp, ap);
      profit.back().insert(profit.back().end(), prft.begin(), prft.end());

      auto nxtst = config->get_next_state(sp, ap);
      func_state.round(nxtst);
      next_state.back().push_back(nxtst);
    }
  }
}



