#include "model.hpp"

// law of motion
profile model::get_next_state(profile& s, profile& a) {
  if (num_agent!=2) {
    std::cerr << "Only support 2 agents" << std::endl;
    exit(1);
  }

  auto delta1 = get_clientcapital_incfactor(a);
  auto delta2 = get_clientcapital_decfactor(s,a);
    
  profile ret = s;
  for (int i=0; i<num_agent; i++) {
    ret[i][0] = (1+delta1[i]) * (1-delta2[i]) * s[i][0] + a[i][0];
  }
  func_state.round(ret);
  return ret;
}

// profit function
std::vector<double> model::get_profit(profile& s, profile& a) {
  std::vector<double> ret(num_agent);
  auto quantity  = get_quantity(a);
  auto unitcost  = get_unitproduct_cost(a);
  auto cccost    = get_unitclientcapital_cost(a);
  auto setupcost = get_setup_cost(s, a);

  for (int i=0; i<num_agent; i++) {
    ret[i] = quantity[i]* (a[i][2]-unitcost[i])
      - a[i][0] * cccost[i]
      - setupcost[i];
  }
    
  return ret;
}

/////////////////////
// helper function //
/////////////////////

  
double model::F(const double &theta) {
  if (theta>1) 
    return 1.;

  if (theta<0)
    return 0.;

  return theta;
}
  
std::vector<double> model::get_quantity(profile& a) {
  if (num_agent!=2) {
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

std::vector<double> model::get_setup_cost(profile &s, profile &a) {
  std::vector<double> ret(num_agent);
  for (int i=0; i<num_agent; i++) {
    ret[i] = std::pow(a[i][1],2) / 2 / s[i][0];
  }
  return ret;
}

std::vector<double> model::get_unitproduct_cost(profile &a) {
  std::vector<double> ret(num_agent);
  for (int i=0; i<num_agent; i++) {
    ret[i] = a[i][1];
  }
  return ret;
}

std::vector<double> model::get_unitclientcapital_cost(profile &a) {
  return std::vector<double>(num_agent, 1.);
}

std::vector<double> model::get_clientcapital_incfactor(const profile &a) {
  return std::vector<double>(num_agent, 0.1);
}
  
std::vector<double> model::get_clientcapital_decfactor(const profile &s, const profile &a) {
  return std::vector<double>(num_agent, 0.9);
}
