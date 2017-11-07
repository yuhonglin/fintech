#include <iostream>
#include <vector>

#include <fenv.h>

#include "model.hpp"
#include "alg.hpp"

int main(int argc, char *argv[])
{

//  feenableexcept(-1 xor FE_INEXACT);

  /* Fintech */
  // func_arry state({0.1}, {3.0}, {0.1});

  // func_arry act({1., 1., 1.},
  // 		{2.1, 2.1, 2.1},
  // 		{0.5, 0.5, 0.5});
    
  // model mod;
  // mod.set_num_agent(2)
  //    .set_num_normal(4)
  //    .set_func_action(func_prof({act, act}))
  //    .set_func_state(func_prof({state, state}))
  //    .set_beta({0.1, 0.1})
  //    .set_init_constant(5);

  // alg a(mod);
  // a.output_dir = "/home/honglin/tmp/";
  // a.solve();



  /* Prison Dilemma */
  // func_arry state({0.1}, {1.0}, {2.0}); // only one state

  // func_arry act({1.},        // only two possible actions  
  // 		{1.9},   
  // 		{0.5});
    
  // model mod;
  // mod.set_num_agent(2)
  //   .set_num_normal(12)
  //   .set_func_action(func_prof({act, act}))
  //   .set_func_state(func_prof({state, state}))
  //   .set_beta({0.8, 0.8})
  //   .set_init_constant(8);

  // alg a(mod);
  // a.output_dir = "/home/honglin/tmp/";
  // a.solve();

  /* Oligopoly */

}
