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
private:
  func_prof func_state;
  func_prof func_action;

  int num_agent_;
  int num_normal_;

  double init_constant_;
  
  // discount factor for each firm;
  std::vector<double> beta_;

  /* Oligopoly model */
  int nk;
  double delta;
  std::vector<double> c;
  double cf;
  double cm;
  double ps;
  double rho;

  //////////////
  // iterface //
  //////////////
public:  
  // law of motion
  profile get_next_state(profile& s, profile& a);

  // profit function
  std::vector<double> get_profit(profile& s, profile& a);

  // get action functors
  func_prof get_action_func(profile& state);

  // get action functors
  func_prof get_state_func();

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
