#include "FinTech.hpp"

// law of motion
profile FinTech::get_next_state(profile& s, profile& a) {
  if (num_agent_!=2) {
    std::cerr << "Only support 2 agents" << std::endl;
    exit(1);
  }

  auto delta1 = get_clientcapital_incfactor(a);
  auto delta2 = get_clientcapital_decfactor(s,a);
  
  profile ret = s;
  for (int i=0; i<num_agent_; i++) {
    ret[i][0] = (1+delta1[i]) * (1-delta2[i]) * s[i][0] + a[i][0];
  }

  return ret;
}

// profit function
std::vector<double> FinTech::get_profit(profile& s, profile& a) {
  /* FinTech model */
  std::vector<double> ret(num_agent_);
  auto quantity  = get_quantity(a);
  auto unitcost  = get_unitproduct_cost(a);
  auto cccost    = get_unitclientcapital_cost(a);
  auto setupcost = get_setup_cost(s, a);

  for (int i=0; i<num_agent_; i++) {
    ret[i] = quantity[i]* (a[i][2]-unitcost[i])
      - a[i][0] * cccost[i]
      - setupcost[i];
  }
  return ret;
}

// States can determine the actions
func_prof FinTech::get_action_func(profile& s) {
  return func_action;
}

// States can determine the actions
func_prof FinTech::get_state_func() {
  return func_state;
}

/////////////////////
// helper function //
/////////////////////
  
double FinTech::F(const double &theta) {
  if (theta>1) 
    return 1.;

  if (theta<0)
    return 0.;

  return theta;
}
  
std::vector<double> FinTech::get_quantity(profile& a) {
  if (num_agent_!=2) {
    std::cerr << "Only support 2 agents" << std::endl;
    exit(1);
  }

  std::vector<double> ret(2);
  if (a[0][1] < a[1][1]) {
    ret[0] = F( (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]) ) - F( a[0][2] / a[0][1] );
    ret[1] = 1 - F( (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]) );
  } else if (a[0][1] > a[1][1]) {
    ret[0] = 1 - F( (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]) );
    ret[1] = F( (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]) ) - F( a[1][2]/a[1][1] );
  } else {
    double tmp = 1 - F( a[0][2] / a[0][1] );
    ret[0] = .5*tmp;
    ret[1] = ret[0];
  }

  return ret;
}


std::vector<double> FinTech::get_setup_cost(profile &s, profile &a) {
  std::vector<double> ret(num_agent_);
  // profile next_state = get_next_state(s, a);
  for (int i=0; i<num_agent_; i++) {
    // ret[i] = std::pow(a[i][1],2) / 2 / next_state[i][0]; // CHECK: should use next state's client capital
    ret[i] = std::pow(a[i][1],2) / 2 / (1 + s[i][0]);
    // ret[i] = std::abs(a[i][1]) / 2 / (1+s[i][0]);
  }
  return ret;
}

std::vector<double> FinTech::get_unitproduct_cost(profile &a) {
  std::vector<double> ret(num_agent_);
  for (int i=0; i<num_agent_; i++) {
    // ret[i] = a[i][1];
    ret[i] = 0.1*a[i][1];
  }
  return ret;
}

std::vector<double> FinTech::get_unitclientcapital_cost(profile &a) {
  return std::vector<double>(num_agent_, 1.);
  // return std::vector<double>(num_agent_, 0.2);
}

std::vector<double> FinTech::get_clientcapital_incfactor(const profile &a) {
  return std::vector<double>(num_agent_, 0.1);
  // return std::vector<double>(num_agent_, 0.);
}
  
std::vector<double> FinTech::get_clientcapital_decfactor(const profile &s, const profile &a) {
  return std::vector<double>(num_agent_, 0.9);
  // return std::vector<double>(num_agent_, 0.08);
  // return std::vector<double>(num_agent_, 0.92);
}
