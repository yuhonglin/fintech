#include "FinTech.hpp"

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
  
  return ret;
}

std::vector<double> FinTech::get_setup_cost(profile &s, profile &a) {
  std::vector<double> ret(num_agent_);
  // profile next_state = get_next_state(s, a);
  for (int i=0; i<num_agent_; i++) {
    if (s[i][0] != 0)
      ret[i] = std::pow(a[i][1],2) / 2 / std::abs(s[i][0]); //
    else
      ret[i] = 30; // return a big number
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

  const double entry_cost                = 0.5;
  const double client_capital_unit_price = 0.2;
  const double maintenance_unit_cost     = 0.1;
  const double scrap_value               = 0.3;
  
  for (int i = 0; i < num_agent_; i++) {
    ret[i] = 0.;
    
    const double &k    = sp[i][0];
    const double &nxtk = ap[i][0];

    if (k == 0) {
      if (nxtk > 0) {
	ret[i] = entry_cost + client_capital_unit_price*nxtk;
      } else {
	ret[i] = 0.;
      }
    } else {
      if (nxtk >= k) {
	ret[i] = client_capital_unit_price*(nxtk-k) + maintenance_unit_cost*k;
      } else if (k > nxtk and nxtk > 0) {
	ret[i] = maintenance_unit_cost*nxtk;
      } else {
	ret[i] = - scrap_value;
      }
    }
  }

  return ret;
}
