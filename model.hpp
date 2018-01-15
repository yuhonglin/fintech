#ifndef MODEL_H
#define MODEL_H

#include <numeric>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>

#include "array.hpp"
#include "profile.hpp"
#include "funcarry.hpp"
#include "funcprof.hpp"


struct model {
protected:
  func_prof func_state;
  func_prof func_action;

  int num_agent_;
  int num_normal_;

  double init_constant_;
  
  // discount factor for each firm;
  std::vector<double> beta_;
  
public:  
  // return the law-of-motion of states.
  virtual profile get_next_state(profile& s, profile& a) = 0;
  // return the profit in a stage game.
  virtual std::vector<double> get_profit(profile& s, profile& a) = 0;
  // return action functors
  virtual func_prof get_action_func(profile& state) = 0;
  // return action functors
  virtual func_prof get_state_func() = 0;

  // getters
  const int&    num_agent() const;
  const int&    num_normal() const ;    
  const double& init_constant() const;
  const std::vector<double>& beta() const;

  // setters
  model& set_num_agent(const int& i);
  model& set_num_normal(const int& i);  
  model& set_beta(const std::vector<double>&);
  model& set_init_constant(const double& c);
  model& set_func_state(const func_prof& fs);
  model& set_func_action(const func_prof& fa);

  // helper functions
  std::pair<std::vector<double>, std::vector<double>>
  stage_profit_bound(); // return maximum possible stage profit for each player
};


#endif /* MODEL_H */
