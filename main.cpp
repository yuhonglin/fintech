#include <iostream>
#include <vector>

#include "model.hpp"
#include "alg.hpp"


int main(int argc, char *argv[])
{
  func_arry state({0.1}, {3.0}, {0.1});

  func_arry act({1., 1., 1.},
		{2.1, 2.1, 2.1},
		{0.5, 0.5, 0.5});
    
  model mod;
  mod.num_agent = 2;
  mod.num_normal = 4;
  mod.func_action = func_prof({act, act});
  mod.func_state = func_prof({state, state});
  mod.beta = {0.3, 0.3};
  mod.init_w = 1000;

  alg a(mod);

  // for (auto i = mod.func_state.begin();
  //      i != mod.func_state.end();
  //      mod.func_state.inc(i))
  //     std::cout << i << std::endl;
  
  
  a.solve();

  // auto tmpfunc = func_prof({state, state});
  // profile tmpprof = tmpfunc.begin();
  // std::cout << state.card() << std::endl;
  // std::cout << act.card() << std::endl;
  // std::cout << func_prof({act, act}).card() << std::endl;
  // tmpprof[0][0] = 2.11;
  // tmpprof[0][0] = 1.48;
  // tmpfunc.round(tmpprof);
  // std::cout << tmpprof << std::endl;
}
