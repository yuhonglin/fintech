#include <iostream>
#include <vector>

#include <fenv.h>

#include "model.hpp"
#include "alg.hpp"
#include "PrisonDilemma.hpp"
#include "Oligopoly.hpp"
#include "FinTech.hpp"

int main(int argc, char *argv[])
{

//  feenableexcept(-1 xor FE_INEXACT);

  // //////////////
  // // Fintech  //
  // //////////////
  
  func_arry state({1.0}, {5.1}, {0.1});

  func_arry act({1., 1., 1.},
  		{3.1, 2.1, 3.1},
  		{0.5, 0.5, 0.5});
    
  FinTech mod;
  mod.set_num_agent(2)
     .set_num_normal(4)
     .set_func_action(func_prof({act, act}))
     .set_func_state(func_prof({state, state}))
     .set_beta({0.8, 0.8})
     .set_init_constant(100);

  alg a(&mod);
  a.output_dir = "./tmp/";
  a.solve();



  ////////////////////
  // Prison Dilemma //
  ////////////////////
  
  // func_arry state({0.1}, {1.0}, {2.0}); // only one state

  // func_arry act({1.},        // only two possible actions  
  // 		{1.9},   
  // 		{0.5});
    
  // PrisonDilemma mod;
  // mod.set_num_agent(2)
  //   .set_num_normal(12)
  //   .set_func_action(func_prof({act, act}))
  //   .set_func_state(func_prof({state, state}))
  //   .set_beta({0.8, 0.8})
  //   .set_init_constant(8);

  // alg a(&mod);
  // a.output_dir = "./tmp/";
  // a.solve();
  


  ///////////////
  // Oligopoly //
  ///////////////
     
  // func_arry state({0.}, {5.01}, {1.0});

  // func_arry act({0., 0.},
  //  		{5.01, 5.01},
  //  		{0.1, 1.0});
    
  // Oligopoly mod;
  // mod.set_num_agent(2)
  //   .set_num_normal(4)
  //   .set_func_action(func_prof({act, act}))
  //   .set_func_state(func_prof({state, state}))
  //   .set_beta({0.8, 0.8})
  //   .set_init_constant(30);

  // mod.c   = {0.9, 0.9};
  // mod.cf  = 2.5;
  // mod.cm  = 1.5;
  // mod.ps  = 1.5;
  // mod.rho = 3;
  // mod.G   = 6;
  // mod.g   = 0.3;

  // alg a(&mod);
  // a.output_dir = "/home/honglin/tmp/";
  // a.solve();


  ////////////////
  // some test  //
  ////////////////
  
  // func_arry act1({1., 0., 0.},  
  // 		 {3.001, 5.01, 5.01},
  // 		 {0.01, 0.1, 1.0});
  // func_prof fa1({act1, act1});
  // fa1.set_ub({2.001, 2.001}, 0);
  
  // func_arry act2({1., 0., 0.},  
  // 		 {2.001, 5.01, 5.01},
  // 		 {0.01, 0.1, 1.0});
  // func_prof fa2({act2,act2});
  
  // std::cout << (fa1==fa2) << std::endl;

  // func_arry act1({1., 0., 0.},  
  //  		 {3.001, 5.01, 5.01},
  //  		 {0.3, 0.1, 1.0});
  // func_prof fa1({act1, act1});

  // profile i;
  // for (i = fa1.begin(); i!= fa1.end(); fa1.inc(i)) {
  //   std::cout << i << std::endl;
  // }

  // profile i;
  // for (i = fa1.begin(); i!= fa1.end(); fa1.inc_only(i, 0)) {
  //   fa1.round(i);
  //   std::cout << i << std::endl;
  // }
}
