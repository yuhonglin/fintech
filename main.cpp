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

  // //////////////
  // // Fintech  //
  // //////////////
  
  func_arry state({0.0}, {5.01}, {1.0});

  func_arry act({0., 0.0, 0.0},
  		{5.01, 5.01, 5.01},
  		{1., 1., 1.}); 

  
  FinTech mod;
  mod.set_num_agent(2)
    .set_num_normal(36)
    .set_func_action(func_prof({act, act}))
    .set_func_state(func_prof({state, state}))
    .set_beta({0.8, 0.8});

  // // some test
  // func_prof fa({act, act});
  // func_prof fs({state, state});

  // auto sp = fs.begin();
  // auto ap = fa.begin();

  // std::cout << sp << std::endl;
  // std::cout << ap << std::endl;
  // std::cout << mod.get_profit(sp, ap) << std::endl;
  // return 0;
  // // end of test


  alg a(&mod);
  a.output_dir = "./tmp/";
  a.solve();


}
