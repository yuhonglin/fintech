// #include "model.hpp"

// // law of motion
// profile model::get_next_state(profile& s, profile& a) {
//   /* FinTech model */
//   // if (num_agent_!=2) {
//   //   std::cerr << "Only support 2 agents" << std::endl;
//   //   exit(1);
//   // }

//   // auto delta1 = get_clientcapital_incfactor(a);
//   // auto delta2 = get_clientcapital_decfactor(s,a);
    
//   // profile ret = s;
//   // for (int i=0; i<num_agent_; i++) {
//   //   ret[i][0] = (1+delta1[i]) * (1-delta2[i]) * s[i][0] + a[i][0];
//   // }
//   // func_state.round(ret);
//   // return ret;

//   /* prinson dilemma */
//   // return s;

//   /* Oligopoly model */
//   profile ret = s;
//   for (int i=0; i<num_agent_; i++) {
//     ret[i][0] = a[i][1];
//   }
//   return ret;
// }

// // profit function
// std::vector<double> model::get_profit(profile& s, profile& a) {

//   /* FinTech model */
//   // std::vector<double> ret(num_agent_);
//   // auto quantity  = get_quantity(a);
//   // auto unitcost  = get_unitproduct_cost(a);
//   // auto cccost    = get_unitclientcapital_cost(a);
//   // auto setupcost = get_setup_cost(s, a);

//   // for (int i=0; i<num_agent_; i++) {
//   //   ret[i] = quantity[i]* (a[i][2]-unitcost[i])
//   //     - a[i][0] * cccost[i]
//   //     - setupcost[i];
//   // }
//   // return ret;

  
//   /* prinson dilemma */  
//   // double act1 = a[0][0];
//   // double act2 = a[1][0];
//   // std::vector<double> ret(num_agent_);
//   // if (act1 == 1. and act2 == 1.) {
//   //   ret[0] = 4;
//   //   ret[1] = 4;
//   // } else if (act1==1.0 and act2==1.5) {
//   //   ret[0] = 0;
//   //   ret[1] = 6;
//   // } else if (act1==1.5 and act2==1.0) {
//   //   ret[0] = 6;
//   //   ret[1] = 0;
//   // } else if (act1==1.5 and act2==1.5) {
//   //   ret[0] = 2;
//   //   ret[1] = 2;
//   // } else {
//   //   std::cerr << "ERROR: unknown action" << std::endl;
//   // }
//   // return ret;

  
//   /* Oligopoly model */
  
// }

// // States can determine the actions
// func_prof model::get_action_func(profile& s) {
//   /* FinTech model */
//   // return func_action; // do nothing

//   /* prinson dilemma */
//   // return func_action; // do nothing

//   /* Oligopoly model */
//   func_prof ret = func_action;
//   for (int i=0; i<num_agent_; i++) {
//     ret[i].set_ub(s[i][0], 0);
//   }
//   return ret;
// }

// // States can determine the actions
// func_prof model::get_state_func() {
//   return func_state;
// }

// /////////////////////
// // helper function //
// /////////////////////

  
// double model::F(const double &theta) {
//   if (theta>1) 
//     return 1.;

//   if (theta<0)
//     return 0.;

//   return theta;
// }
  
// std::vector<double> model::get_quantity(profile& a) {
//   if (num_agent_!=2) {
//     std::cerr << "Only support 2 agents" << std::endl;
//     exit(1);
//   }

//   std::vector<double> ret(2);
//   if (a[0][1] < a[1][1]) {
//     ret[0] = F( (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]) ) - F( a[0][2] / a[0][1] );
//     ret[1] = 1 - F( (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]) );
//   } else if (a[0][1] > a[1][1]) {
//     ret[0] = 1 - F( (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]) );
//     ret[1] = F( (a[0][2]-a[1][2]) / (a[0][1]-a[1][1]) ) - F( a[1][2]/a[1][1] );
//   } else {
//     double tmp = 1 - F( a[0][2] / a[0][1] );
//     ret[0] = .5*tmp;
//     ret[1] = ret[0];
//   }

//   return ret;
// }



// /////////////////////////
// // getters and setters //
// /////////////////////////

// std::vector<double> model::get_setup_cost(profile &s, profile &a) {
//   std::vector<double> ret(num_agent_);
//   for (int i=0; i<num_agent_; i++) {
//     ret[i] = std::pow(a[i][1],2) / 2 / s[i][0];
//   }
//   return ret;
// }

// std::vector<double> model::get_unitproduct_cost(profile &a) {
//   std::vector<double> ret(num_agent_);
//   for (int i=0; i<num_agent_; i++) {
//     ret[i] = a[i][1];
//   }
//   return ret;
// }

// std::vector<double> model::get_unitclientcapital_cost(profile &a) {
//   return std::vector<double>(num_agent_, 1.);
// }

// std::vector<double> model::get_clientcapital_incfactor(const profile &a) {
//   return std::vector<double>(num_agent_, 0.1);
// }
  
// std::vector<double> model::get_clientcapital_decfactor(const profile &s, const profile &a) {
//   return std::vector<double>(num_agent_, 0.9);
// }

// const int& model::num_agent() const {
//   return num_agent_;
// }

// const int& model::num_normal() const {
//   return num_normal_;
// }

// const double& model::init_constant() const {
//   return init_constant_;
// }

// const std::vector<double>& model::beta() const {
//   return beta_;
// }

// model& model::set_num_agent(const int& i) {
//   num_agent_ = i;
//   return *this;
// }

// model& model::set_num_normal(const int& i) {
//   num_normal_ = i;
//   return *this;
// }

// model& model::set_beta(const std::vector<double>& b) {
//   beta_ = b;
//   return *this;
// }

// model& model::set_init_constant(const double& c) {
//   init_constant_ = c;
//   return *this;
// }

// model& model::set_func_state(const func_prof& fs) {
//   func_state = fs;
//   return *this;
// }

// model& model::set_func_action(const func_prof& fa) {
//   func_action = fa;
//   return *this;
// }
