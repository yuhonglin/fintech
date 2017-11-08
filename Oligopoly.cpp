#include "Oligopoly.hpp"

// law of motion
profile Oligopoly::get_next_state(profile& s, profile& a) {
  profile ret = s;
  for (int i=0; i<num_agent_; i++) {
    ret[i][0] = a[i][1];
  }
  return ret;
}

// profit function
std::vector<double> Oligopoly::get_profit(profile& s, profile& a) {
  std::vector<double> ret(num_agent_);
  auto price = get_price(a);
  auto cost  = get_maintain_cost(s, a);
  for (int i=0; i<num_agent_; i++) {
    ret[i] = a[i][0]*(price - c[i]) - cost[i];
  }
  return ret;
}

// States can determine the actions
func_prof Oligopoly::get_action_func(profile& s) {

  func_prof ret = func_action;
  std::vector<double> nub(num_agent_);
  for (int i=0; i<num_agent_; i++) nub[i] = rho*s[i][0];
  
  ret.set_ub(nub, 0);

  return ret;
}

// States can determine the actions
func_prof Oligopoly::get_state_func() {
  return func_state;
}


/////////////////////
// helper function //
/////////////////////
  
double Oligopoly::F(const double &theta) {
  if (theta>1) 
    return 1.;

  if (theta<0)
    return 0.;

  return theta;
}
  
double Oligopoly::get_price(profile& a) {
  double sum = 0;
  for (int i=0; i<num_agent_; i++) {
    sum += a[i][0];
  }
  sum = G - g*sum;
  if (sum > 0) {
    return sum;
  } else {
    return 0.;
  }
}

std::vector<double> Oligopoly::get_maintain_cost(profile &s, profile &a) {
  std::vector<double> ret(num_agent_);
  for (int i=0; i<num_agent_; i++) {
    if (a[i][1] > s[i][0]) {
      ret[i] = cm*s[i][0] + cf*(a[i][1] - s[i][0]);
    } else {
      ret[i] = cm*s[i][0] - cf*(s[i][0] - a[i][1]);
    }
  }
  return ret;
}

