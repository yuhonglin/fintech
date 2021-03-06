#include <limits>
#include <iostream>
#include <cmath>

#include "util.hpp"
#include "lpsolver.hpp"

#define SAFE_PAD 100

lp_solver::lp_solver(int nn, int mm)
  : n(nn), nclin(mm), iw(new int[nn+SAFE_PAD]), m(0),
    leniw(nn), clamda(new double[nn+mm+SAFE_PAD]),
    istate(new int[nn+mm+SAFE_PAD]),
    kx(new int[1]), R(new double[1]), b(new double[1]) {

  // allocate w
  if (n <= nclin)
    lenw = 2*n*n+7*n+6*nclin + SAFE_PAD;
  else
    lenw = 2*(nclin+1)*(nclin+1) + 7*n+6*nclin + SAFE_PAD;
  w.reset(new double[lenw]);

  // set init value of istate
  for (int i=0; i<n+nclin; i++) {
    istate[i] = 0;
  }

}

void lp_solver::solve(double* c, double* A, // inputs
		      double* lb, double* ub, int cold, // inputs
		      double& obj, double* sol, STATUS& stat) /* outputs */ {

#define USE_LSSOL
#ifdef USE_LSSOL
  
  int inform;
  int ldR = 0;

  lpsol_(&m, &n, &nclin, &nclin, &ldR, A, lb, ub, c, istate.get(), kx.get(), sol, R.get(), b.get(), &inform, &iter, &obj, clamda.get(), iw.get(), &leniw, w.get(), &lenw, &cold);

  if (inform == 3) {
    stat = INFEASIBLE;
    obj = std::numeric_limits<double>::quiet_NaN();
  } else if (inform == 0) {
    stat = SUCCESS;
  } else if (inform == 1){
    if (std::isnan(obj)) {
      stat = INFEASIBLE;
    } else {
      stat = SUCCESS;
    }
  } else if (inform == 4 || inform == 5) {
    // exceeds maximum iteration
    // shift the problem a bit
    // based on some test for such cases, MATLAB's linprog's error message is infeasible
    // so here this error is treated as infeasible
    //  std::cout << "inform = " << inform
    // 	       << ", maxiter reached, the problem is:" << std::endl;
    // // // output put the problem
    // std::cout << "c = [";
    // for (int i=0; i<n; i++) {
    //   std::cout << c[i] << ',';
    // }
    // std::cout << "]\n";

    // std::cout << "lb = [";
    // for (int i=0; i<(n+nclin); i++) {
    //   std::cout << lb[i] << ',';
    // }
    // std::cout << "]\n";

    // std::cout << "ub = [";
    // for (int i=0; i<(n+nclin); i++) {
    //   std::cout << ub[i] << ',';
    // }
    // std::cout << "]\n";

    // std::cout << "A = [\n";
    // for (int i=0; i<nclin; i++) {
    //   for (int j=0; j<n; j++) {
    // 	std::cout << A[i+j*nclin] << ',';
    //   }
    //   std::cout << std::endl;
    // }
    // std::cout << "]\n";

    // util::dump(c , n, "mat/c");
    // util::dump(lb, n+nclin, "mat/lb");
    // util::dump(ub, n+nclin, "mat/ub");
    // util::dump(A , n*nclin, "mat/A");
    //     std::cin.get();

    // stat = ERROR;
    stat = INFEASIBLE;
  } else {
     stat = ERROR;
    //stat = INFEASIBLE;
  }

#else
  if (nclin!=4) {
    std::cerr << "only support m=4, not " << nclin << std::endl;
    exit(1);
  }
  //solve by basic method when n == 2
  double f = 0.;
  for (int i = 0; i < n; i++) {
    double l = std::max(lb[i], -ub[4+i]);
    double u = std::min(ub[i],  ub[2+i]);
    if (l > u) {
      // infeasible
      stat = INFEASIBLE;
      return;
    }
    if (c[i] > 0) {
      f += c[i]*l;
      sol[i] = l;
    } else {
      f += c[i]*u;
      sol[i] = u;
    }
  }
  obj = f;
      
#endif
      
}

