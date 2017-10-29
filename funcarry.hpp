#ifndef FUNCARRY_H
#define FUNCARRY_H

#include <numeric>
#include <iostream>
#include <cmath>
#include <vector>

#include "array.hpp"

struct func_arry {
  // parameters
private:
  std::vector<double> lb;
  std::vector<double> ub;
  std::vector<double> stp;

  array end_;

  int card_;

  int card_single(double l, const double& u, const double& s) {
    int ret = 0;
    while(l <= u) {
      ret++;
      l += s;
    };
    return ret;
  };
    
public:
  func_arry(const std::vector<double>& l,
	    const std::vector<double>& u,
	    const std::vector<double>& s) {
    lb   = l;
    ub   = u;
    stp  = s;
	
    card_ = 1;
    for (int i=0; i<l.size(); i++) {
      card_ *= card_single(lb[i], ub[i], stp[i]);
    }
  };
    
  const array& end() const {
    return end_;
  };

  array begin() const {
    return array(lb);
  };
    
  void inc(array& a) const {
    for (int i=a.size()-1; i>=0; i--) {
      if (a[i] + stp[i] > ub[i]) {
	a[i] = lb[i];
	continue;
      } else {
	a[i] += stp[i];
	a++;
	return;
      }
    }
    a = end();
    return;
  };

  void reset(array& a) const {
    a.reset(lb);
  };

  int card() const {
    return card_;
  };
};

#endif /* FUNCARRY_H */
