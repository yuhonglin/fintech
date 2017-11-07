// #include "model.hpp"

// class Oligopoly : public model
// {
//   /* Oligopoly model */
//   int nk;
//   double delta;
//   std::vector<double> c;
//   double cf;
//   double cm;
//   double ps;
//   double rho;
  
// public:
//   Oligopoly();
//   virtual ~Oligopoly();

//   //////////////
//   // iterface //
//   //////////////
//   // law of motion
//   virtual profile get_next_state(profile& s, profile& a);

//   // profit function
//   virtual std::vector<double> get_profit(profile& s, profile& a);

//   // get action functors
//   virtual func_prof get_action_func(profile& state);

//   // get action functors
//   virtual func_prof get_state_func();


//   /////////////////////
//   // helper function //
//   /////////////////////

//   double F(const double &theta);
  
//   std::vector<double> get_quantity(profile& a);

//   std::vector<double> get_setup_cost(profile &s, profile &a);

//   std::vector<double> get_unitproduct_cost(profile &a);

//   std::vector<double> get_unitclientcapital_cost(profile &a);

//   std::vector<double> get_clientcapital_incfactor(const profile &a);
  
//   std::vector<double> get_clientcapital_decfactor(const profile &s, const profile &a);
  
// };
