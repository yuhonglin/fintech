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

  if (a[0][1] <= 0.) {
    ret[0] = 0.;
    if (a[1][1] <= 0.) {
      ret[1] = 0.;
      return ret;
    } else {
      ret[1] = 1 - F( a[1][2] / a[1][1] );
      return ret;
    }
  } else if (a[1][1] <= 0.) {
    ret[0] = 1 - F( a[0][2] / a[0][1] );
    ret[1] = 0.;
    return ret;
  }

  double t1 = a[0][2] / a[0][1];
  double t2 = a[1][2] / a[1][1];
  double tm = (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]);

  if (a[0][1] < a[1][1]) {
    if (tm <= t2) {
      // [t2,1] will buy firm 2
      ret[1] = 1 - F(t2);
      if (t1 <= tm) {
	// [t1,tm] will buy firm 1
	ret[0] = F(tm) - F(t1);	
      } else {
	// no one buy firm 1
	ret[0] = 0.;       
      }
    } else {
      // [tm,1] will buy firm 2
      ret[1] = 1 - F(tm);
      if (t1 <= tm) {
	// [t1,tm] will buy firm 1
	ret[0] = F(tm) - F(t1);
      } else {
	// no one buy firm 1
	ret[0] = 0.;
      }
    }
  } else if (a[0][1] > a[1][1]) {
    if (tm <= t1) {
      // [t1,1] will buy firm 1
      ret[0] = 1 - F(t1);
      if (t2 <= tm) {
	// [t2,tm] will buy firm 2
	ret[1] = F(tm) - F(t2);
      } else {
	// no one will buy firm 2
	ret[1] = 0.;
      }
    } else {
      // [tm,1] will buy firm 1
      ret[0] = 1 - F(tm);
      if (t2 <= tm) {
	// [t2,tm] will buy firm 2
	ret[1] = F(tm) - F(t2);
      } else {
	// no one will buy firm 2
	ret[1] = 0.;
      }
    }
  } else {
    if (a[0][2] > a[1][2]) {
      ret[0] = 0.;
      ret[1] = 1 - F( t2 );
    } else if (a[0][2] < a[1][2]) {
      ret[0] = 1 - F( t1 );
      ret[1] = 0.;
    } else {
      ret[0] = 0.5*( 1 - F( t1 ) );
      ret[1] = ret[0];
    }
  }
  
  // if (a[0][1] < a[1][1]) {
  //   ret[0] = F( (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]) ) - F( a[0][2] / a[0][1] );
  //   ret[1] = 1 - F( (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]) );
  // } else if (a[0][1] > a[1][1]) {
  //   ret[0] = 1 - F( (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]) );
  //   ret[1] = F( (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]) ) - F( a[1][2]/a[1][1] );
  // } else {
  //   // double tmp = 1 - F( a[0][2] / a[0][1] );
  //   // ret[0] = .5*tmp;
  //   // ret[1] = ret[0];
  //   if (a[0][2] > a[1][2]) {
  //     ret[0] = 0;
  //     ret[1] = 1 - F( a[1][2] / a[1][1] );
  //   } else if (a[0][2] < a[1][2]) {
  //     ret[0] = 1 - F( a[0][2] / a[0][1] );
  //     ret[1] = 0;
  //   } else {
  //     ret[0] = 0.5*( 1 - F( a[0][2] / a[0][1] ) );
  //     ret[1] = ret[0];
  //   }
  // }

    //  if (ret[0] < 0 || a[0][1] <= 0.) ret[0] = 0.;
    //  if (ret[1] < 0 || a[1][1] <= 0.) ret[1] = 0.;
  
  return ret;
}


std::vector<double> FinTech::get_setup_cost(profile &s, profile &a) {
  std::vector<double> ret(num_agent_);
  // profile next_state = get_next_state(s, a);
  for (int i=0; i<num_agent_; i++) {
    ret[i] = std::pow(a[i][1],2) / 2 / (0.1+s[i][0]); //
   }
  return ret;
}

std::vector<double> FinTech::get_unitproduct_cost(profile &a) {
  std::vector<double> ret(num_agent_);
  for (int i=0; i<num_agent_; i++) {
    ret[i] = .1*a[i][1];
  }
  return ret;
}

std::vector<double> FinTech::get_unitclientcapital_cost(profile &a) {
  return std::vector<double>(num_agent_, 1.);
}

std::vector<double> FinTech::get_clientcapital_incfactor(const profile &a) {
  return std::vector<double>(num_agent_, .1);
}
  
std::vector<double> FinTech::get_clientcapital_decfactor(const profile &s, const profile &a) {
  return std::vector<double>(num_agent_, .3);
}

// double FinTech::cc_decfactor() { return cc_decfactor_; }
// double FinTech::cc_incfactor() { return cc_incfactor_; }
// double FinTech::unit_cc_cost() { return unit_cc_cost_; }
// double FinTech::unit_prod_cost_factor() { return unit_prod_cost_factor_; }

// namespace std {
//   ostream& operator<< (ostream& o, ::FinTech& a) {
    
//     return o;
//   };
// }
