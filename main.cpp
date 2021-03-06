#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#include <fenv.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>

#include "util.hpp"

#include "modelcache.hpp"
#include "model.hpp"
#include "alg.hpp"
#include "PrisonDilemma.hpp"
#include "Oligopoly.hpp"
#include "FinTech.hpp"

namespace std {
  template <class T>
  std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    for (int i = 0; i < v.size(); i++) {
      os << v[i] << '\t';
    }
    return os;
  }
}


int main(int argc, char *argv[])
{

  // func_arry state({0.0}, {4.31}, {0.1});

  // func_arry act({0., 0.0, 0.0},
  // 		{1.21, 1.61, 1.61},
  // 		{0.25, 0.4, 0.4});

  // func_prof fs({state, state});
  // func_prof fa({act, act});

  // FinTech mod;
  // mod.set_num_agent(2)
  //    .set_num_normal(12)
  //    .set_func_action(func_prof({act, act}))
  //    .set_func_state(func_prof({state, state}))
  //    .set_beta({0.8, 0.8})
  //    .set_init_constant(0.3);

  // profile sp, ap;
  // array s1, s2, a1, a2;
  // a1.value = {0, 1.6, 0.8};
  // a2.value = {0,   0, 0.8};  
  // ap.value = {a1, a2};
  // fa.round(ap);
  // std::cout << ap << std::endl;

  // s1.value = {3.9};
  // s2.value = {4.3};
  // sp.value = {s1, s2};
  // fs.round(sp);
  // std::cout << sp << std::endl;

  // {
  //   auto tmp = mod.get_next_state(sp, ap);
  //   std::cout << tmp[0] << '\t' << tmp[1] << std::endl;
  // }

  // {
  //   auto tmp = mod.get_quantity(ap);
  //   std::cout << tmp[0] << '\t' << tmp[1] << std::endl;
  // }

  

//  feenableexcept(-1 xor FE_INEXACT);

  // // //////////////
  // // // Fintech  //
  // // //////////////
  
  // func_arry state({0.0}, {2.701}, {0.1});

  // func_arry act({0., 0.0, 0.0},
  //  		{0.601, 0.501, 0.501},
  //  		{0.2, 0.1, 0.1});

  
  func_arry state({0.0}, {4.501}, {0.1});

  func_arry act({0., 0.0, 0.0},
  		{1.001, 1.61, 1.61},
  		{0.2, 0.8, 0.8}); 



  
  FinTech mod;
  mod.set_num_agent(2)
    .set_num_normal(36)
    .set_func_action(func_prof({act, act}))
    .set_func_state(func_prof({state, state}))
    .set_beta({0.8, 0.8});

  alg a(&mod);
  a.output_dir = "./tmp/";
  a.solve();


  // // test model cache
  // ModelCache cache(&mod);
  // util::tic();
  // cache.build();
  // util::toc();

  // func_prof func_state({state, state});
  // func_prof func_action({act, act});

  // auto sp = func_state.begin() ; for (int i=0; i<19; i++) func_state.inc(sp);
  // auto ap = func_action.begin(); for (int i=0; i<121; i++) func_action.inc(ap);

  // std::cout << sp << std::endl;
  // std::cout << ap << std::endl;
  
  // auto out = mod.get_next_state(sp, ap); func_state.round(out);
  // std::cout << out << std::endl;
  // out = cache.get_next_state(sp, ap);  
  // std::cout << out << std::endl;

  // auto prft = mod.get_profit(sp, ap);
  // std::cout << prft[0] << '\t' << prft[1] << std::endl;
  // auto cprft = cache.get_profit(sp, ap);
  // std::cout << cprft[0] << '\t' << cprft[1] << std::endl;


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
     
  // func_arry state({0.}, {3.01}, {1.0});

  // func_arry act({0., 0.},
  //  		{3.01, 3.01},
  //  		{0.1, 1.0});
    
  // Oligopoly mod;
  // mod.set_num_agent(2)
  //   .set_num_normal(36)
  //   .set_func_action(func_prof({act, act}))
  //   .set_func_state(func_prof({state, state}))
  //   .set_beta({0.8, 0.8})
  //   .set_init_constant(18);

  // mod.c   = {0.9, 0.9};
  // mod.cf  = 2.5;
  // mod.cm  = 1.5;
  // mod.ps  = 1.5;
  // mod.rho = 3;
  // mod.G   = 6;
  // mod.g   = 0.3;

  // alg a(&mod);
  // a.output_dir = "./tmp/";
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
