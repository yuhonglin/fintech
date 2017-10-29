#include <iostream>
#include <vector>

#include "model.hpp"
#include "alg.hpp"


int main(int argc, char *argv[])
{
  func_arry state({0.1}, {0.1}, {3.0});

  func_arry act({0.1, 0.1, 0.1},
		{0.1, 0.1, 0.1},
		{2.0, 2.0, 2.0});
    
  model mod;
  mod.num_agent = 2;
  mod.num_normal = 4;
  mod.func_action = func_prof({act, act});
  mod.func_state = func_prof({state, state});
  mod.beta = {0.3, 0.3};
  
  alg a(mod);
}
