#include "FinTech.hpp"

// 5.1 If k == 0 and k' > 0: pay an entry cost. The total cost will be
//     "client_capital_price*(k'-k) + entry cost"
// 5.2 If k == 0 and k' == 0: total cost=0
// 5.3 If k > 0 and k' > k: total cost = client_capital_price*(k'-k) + maintenance_cost*k.
// 5.4 If k > 0 and k > k' > 0: total cost = -sale_price*(k-k') + maintenance_cost*k'.
// 5.5 If k > 0 and k' == 0: obtain a scrap value. total cost = -sale_price*k - scrap value.
//
// Currently, we set sale_price = 0;
std::vector<double> FinTech::get_unitclientcapital_cost(profile& sp, profile &ap) {
  std::vector<double> ret(num_agent_);

  // Version 1 linear maintenance unit cost
  const std::vector<double> entry_cost                = {0.1, 0.1};
  const std::vector<double> client_capital_unit_price = {0.01, 0.01};
  const std::vector<double> maintenance_unit_cost     = {0.15, 0.15};
  const std::vector<double> scrap_value               = {0.0,0.0};

  
  for (int i = 0; i < num_agent_; i++) {
    ret[i] = 0.;
    
    const double &k    = sp[i][0];
    const double &nxtk = ap[i][0];

    if (k == 0) {
      if (nxtk > 0) {
  	ret[i] = entry_cost[i] + client_capital_unit_price[i]*nxtk;
      } else {
  	ret[i] = 0.;
      }
    } else {
      if (nxtk >= k) {
  	ret[i] = client_capital_unit_price[i]*(nxtk-k) + maintenance_unit_cost[i]*k;
      } else if (k > nxtk and nxtk > 0) {
  	ret[i] = maintenance_unit_cost[i]*nxtk;
      } else {
  	ret[i] = - scrap_value[i];
      }
    }
  }


  // // Version 2 Add some data tax
  // const std::vector<double> entry_cost                = {0.01, 0.01};
  // const std::vector<double> client_capital_unit_price = {0.01, 0.01};
  // const std::vector<double> maintenance_unit_cost     = {0.01, 0.01};
  // const std::vector<double> scrap_value               = {0.0,0.0};

  // for (int i = 0; i < num_agent_; i++) {
  //   ret[i] = 0.;
    
  //   const double &k    = sp[i][0];
  //   const double &nxtk = ap[i][0];

  //   if (k == 0) {
  //     if (nxtk > 0) {
  // 	ret[i] = entry_cost[i] + client_capital_unit_price[i]*nxtk;
  //     } else {
  // 	ret[i] = 0.;
  //     }
  //   } else {
  //     if (nxtk >= k) {
  // 	ret[i] = client_capital_unit_price[i]*(nxtk-k) + maintenance_unit_cost[i]*k;
  //     } else if (k > nxtk and nxtk > 0) {
  // 	ret[i] = maintenance_unit_cost[i]*nxtk;
  //     } else {
  // 	ret[i] = - scrap_value[i];
  //     }
  //   }
  // }


  
  return ret;
}

std::vector<double> FinTech::get_unitproduct_cost(profile &a) {
  std::vector<double> ret(num_agent_);

  const std::vector<double> unit_product_cost = {0.01, 0.01};
  
  for (int i=0; i<num_agent_; i++) {
    ret[i] = unit_product_cost[i]*a[i][1];
  }
  return ret;
}

std::vector<double> FinTech::get_setup_cost(profile &s, profile &a) {
  std::vector<double> ret(num_agent_);
  
  // // Version 1: this version uses the previous client capital
  // for (int i=0; i<num_agent_; i++) {
  //   if (a[i][1] == 0) {
  //     ret[i] = 0;
  //     continue;
  //   }
	
  //   if (s[i][0] != 0) {
  //     ret[i] = std::pow(a[i][1],2) / 20 / std::abs(s[i][0]); // 
  //   } else {
  //     ret[i] = 1e2; // return a big number, should not be too big, otherwise, starting set will be too big
  //   }
  //  }

  // // Version 2: this version uses the next client capital
  // profile next_state = get_next_state(s, a);
  // for (int i=0; i<num_agent_; i++) {
  //   if (a[i][1] == 0) {
  //     ret[i] = 0;
  //     continue;
  //   }
	
  //   if (next_state[i][0] != 0) {
  //     ret[i] = std::pow(a[i][1],2) / 20 / std::abs(next_state[i][0]); // this uses the new client capital
  //   } else {
  //     ret[i] = 1e2; // return a big number, should not be too big, otherwise, starting set will be too big
  //   }
  //  }

  // // Version 3: this version uses the next client capital, with different scale
  // profile next_state = get_next_state(s, a);
  // for (int i=0; i<num_agent_; i++) {
  //   if (a[i][1] == 0) {
  //     ret[i] = 0;
  //     continue;
  //   }
	
  //   if (next_state[i][0] != 0) {
  //     ret[i] = std::pow(a[i][1],2) / 5.0 / std::abs(next_state[i][0]); // this uses the new client capital
  //   } else {
  //     ret[i] = 1e2; // return a big number, should not be too big, otherwise, starting set will be too big
  //   }
  //  }


  // // Version 4: this version uses the next client capital and when the client capital is very large, use different curve
  // profile next_state = get_next_state(s, a);
  // for (int i=0; i<num_agent_; i++) {
  //   if (a[i][1] == 0) {
  //     ret[i] = 0;
  //     continue;
  //   }
	
  //   if (next_state[i][0] != 0) {
  //     if (next_state[i][0] <= 2) {
  // 	ret[i] = std::pow(a[i][1],2) / 4.0 / std::abs(next_state[i][0]); // this uses the new client capital
  //     } else {
  // 	ret[i] = std::pow(a[i][1],2) / 3.0 / std::abs(next_state[i][0]); // this uses the new client capital
  //     }
  //   } else {
  //     ret[i] = 1e2; // return a big number, should not be too big, otherwise, starting set will be too big
  //   }
  //  }

  // // Version 5: this version uses the next client capital and use squared
  // profile next_state = get_next_state(s, a);
  // for (int i=0; i<num_agent_; i++) {
  //   if (a[i][1] == 0) {
  //     ret[i] = 0;
  //     continue;
  //   }
	
  //   if (next_state[i][0] != 0) {
  //     ret[i] = std::pow(a[i][1],2) / 5.0 / std::pow(next_state[i][0],2); // this uses the new client capital
  //   } else {
  //     ret[i] = 1e2; // return a big number, should not be too big, otherwise, starting set will be too big
  //   }
  //  }

  // Version 5: this version uses the next client capital and use logistic-like curves
  profile next_state = get_next_state(s, a);
  for (int i=0; i<num_agent_; i++) {
    if (a[i][1] == 0) {
      ret[i] = 0;
      continue;
    }
	
    //    if (s[i][0] != 0) {
    if (next_state[i][0] != 0) {
      // ret[i] = std::pow(a[i][1],2) / 5.0 / (.5 + std::exp(s[i][0]/2.)); // this uses the old client capital      
      ret[i] = std::pow(a[i][1],2) / 5.0 / (.5 + std::exp(next_state[i][0]/2.)); // this uses the new client capital
    } else {
      ret[i] = 1e2; // return a big number, should not be too big, otherwise, starting set will be too big
    }
   }

  
  return ret;
}


// law of motion
profile FinTech::get_next_state(profile& s, profile& a) {
  auto ret = s;
  
  for (int i=0; i<num_agent_; i++) {
    ret[i][0] = a[i][0];
  }

  return ret;
}

// profit function
std::vector<double> FinTech::get_profit(profile& s, profile& a) {
  /* FinTech model */
  std::vector<double> ret(num_agent_);
  auto quantity  = get_quantity(a);
  auto unitcost  = get_unitproduct_cost(a);
  auto cccost    = get_unitclientcapital_cost(s,a);
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

  std::vector<double> ret(2);

  // Meaning of the variables:
  //   a : client capital
  //   w : quality
  //   p : price
  //   r : quantity (return)
  //   Postscripts "1, 2" denote firm 1 and 2 respectively:
  //
  // Input:
  //   a1 denotes the client capital of firm 1,
  //   a2 denotes the client capital of firm 2,
  //   w1 denotes the quality of firm 1,
  //   w2 denotes the quality of firm 2,
  //   p1 denotes the price of firm 1,
  //   p2 denotes the price of firm 2,
  //   
  // Output:
  //   r1 denotes the quantity of firm 1,
  //   r2 denotes the quantity of firm 2.
  
  double& a1 = a[0][0]; double& a2 = a[1][0];
  double& w1 = a[0][1]; double& w2 = a[1][1];
  double& p1 = a[0][2]; double& p2 = a[1][2];
  double& r1 = ret[0];  double& r2 = ret[1];
  
  if (w1 <= 0.) {
    r1 = 0.;
    if (w2 <= 0.) {
      r2 = 0.;
      return ret;
    } else {
      r2 = 1 - F( p2 / w2 );
      return ret;
    }
  } else if (w2 <= 0.) {
    r1 = 1 - F( p1 / w1 );
    r2 = 0.;
    return ret;
  }

  double t1 = p1 / w1;
  double t2 = p2 / w2;
  double tm = (p1-p2) / (w1-w2);

  if (w1 < w2) {
    if (tm <= t2) {
      // [t2,1] will buy firm 2
      r2 = 1 - F(t2);
      if (t1 <= tm) {
	// [t1,tm] will buy firm 1
	r1 = F(tm) - F(t1);	
      } else {
	// no one buy firm 1
	r1 = 0.;       
      }
    } else {
      // [tm,1] will buy firm 2
      r2 = 1 - F(tm);
      if (t1 <= tm) {
	// [t1,tm] will buy firm 1
	r1 = F(tm) - F(t1);
      } else {
	// no one buy firm 1
	r1 = 0.;
      }
    }
  } else if (w1 > w2) {
    if (tm <= t1) {
      // [t1,1] will buy firm 1
      r1 = 1 - F(t1);
      if (t2 <= tm) {
	// [t2,tm] will buy firm 2
	r2 = F(tm) - F(t2);
      } else {
	// no one will buy firm 2
	r2 = 0.;
      }
    } else {
      // [tm,1] will buy firm 1
      r1 = 1 - F(tm);
      if (t2 <= tm) {
	// [t2,tm] will buy firm 2
	r2 = F(tm) - F(t2);
      } else {
	// no one will buy firm 2
	r2 = 0.;
      }
    }
  } else {
    if (p1 > p2) {
      r1 = 0.;
      r2 = 1 - F( t2 );
    } else if (p1 < p2) {
      r1 = 1 - F( t1 );
      r2 = 0.;
    } else {
      r1 = 0.5*( 1 - F( t1 ) );
      r2 = r1;
    }
  }
  
  return ret;
}

