#ifndef FINTECH_H
#define FINTECH_H

#include "model.hpp"

class FinTech : public model
{
// private:
//   double cc_decfactor_ = 0.1; // 0.3
//   double cc_incfactor_ = 0.3; // 0.1
//   double unit_cc_cost_ = 1.; // 1.
//   double unit_prod_cost_factor_ = 0.1; // 0.1
  
public:
  //////////////
  // iterface //
  //////////////
  // law of motion
  virtual profile get_next_state(profile& s, profile& a);

  // profit function
  virtual std::vector<double> get_profit(profile& s, profile& a);

  // get action functors
  virtual func_prof get_action_func(profile& state);

  // get action functors
  virtual func_prof get_state_func();


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

  // double get_cc_decfactor(); // 0.1
  // double get_cc_incfactor(); // 0.3
  // double get_unit_cc_cost(); // 1.
  // double get_unit_prod_cost_factor(); // 0.1

};


#endif /* FINTECH_H */
