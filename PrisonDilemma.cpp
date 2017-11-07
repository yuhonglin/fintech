#include "PrisonDilemma.hpp"

profile PrisonDilemma::get_next_state(profile& s, profile& a) {
  return s;
}

// profit function
std::vector<double> PrisonDilemma::get_profit(profile& s, profile& a) {
  double act1 = a[0][0];
  double act2 = a[1][0];
  std::vector<double> ret(num_agent_);
  if (act1 == 1. and act2 == 1.) {
    ret[0] = 4;
    ret[1] = 4;
  } else if (act1==1.0 and act2==1.5) {
    ret[0] = 0;
    ret[1] = 6;
  } else if (act1==1.5 and act2==1.0) {
    ret[0] = 6;
    ret[1] = 0;
  } else if (act1==1.5 and act2==1.5) {
    ret[0] = 2;
    ret[1] = 2;
  } else {
    std::cerr << "ERROR: unknown action" << std::endl;
  }
  return ret;
}

// States can determine the actions
func_prof PrisonDilemma::get_action_func(profile& s) {
  return func_action; // do nothing
}

// States can determine the actions
func_prof PrisonDilemma::get_state_func() {
  return func_state;
}
