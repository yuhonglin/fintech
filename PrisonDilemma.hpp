#include "model.hpp"

class PrisonDilemma : public model {
  //////////////
  // iterface //
  //////////////
  // law of motion
public:
  virtual profile get_next_state(profile& s, profile& a);

  // profit function
  virtual std::vector<double> get_profit(profile& s, profile& a);

  // get action functors
  virtual func_prof get_action_func(profile& state);

  // get action functors
  virtual func_prof get_state_func();  
};
