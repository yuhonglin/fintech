#include "model.hpp"

class Oligopoly : public model
{
public:
  /* Oligopoly model */
  std::vector<double> c;
  double  cf;
  double  cm;
  double  ps;
  double rho;
  double   G;    // Gamma
  double   g;    // gamma
  
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
  
  double get_price(profile& a);

  std::vector<double> get_maintain_cost(profile &s, profile &a);
  
};
