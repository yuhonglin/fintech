#include <iostream>
#include <vector>

#include <fenv.h>

#include "model.hpp"
#include "alg.hpp"

int main(int argc, char *argv[])
{

//  feenableexcept(-1 xor FE_INEXACT);
  
  func_arry state({0.1}, {3.0}, {0.1});

  func_arry act({1., 1., 1.},
  		{2.1, 2.1, 2.1},
  		{0.5, 0.5, 0.5});
    
  model mod;
  mod.num_agent = 2;
  mod.num_normal = 4;
  mod.func_action = func_prof({act, act});
  mod.func_state = func_prof({state, state});
  mod.beta = {0.9, 0.9};
  mod.init_w = 1000;

  alg a(mod);
  a.solve();



  /* Prison Dilemma */
  // func_arry state({0.1}, {1.0}, {2.0}); // only one state

  // func_arry act({1.},        // only two possible actions  
  // 		{1.9},   
  // 		{0.5});
    
  // model mod;
  // mod.num_agent = 2;
  // mod.num_normal = 4;
  // mod.func_action = func_prof({act, act});
  // mod.func_state = func_prof({state, state});
  // mod.beta = {0.8, 0.8};
  // mod.init_w = 10;

  // alg a(mod);
  // a.solve();
  

}
