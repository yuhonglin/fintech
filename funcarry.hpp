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

  std::vector<double> ub_true;
  
  array end_;
  int card_;

  std::vector<int> prod;

  std::vector<int> card_each;
  
  int card_single(double l, const double& u, const double& s, double& ut) {
    int ret = 0;
    while(l <= u) {
      ret++;
      l += s;
    };
    ut = l;
    return ret;
  };
    
public:
  func_arry(const std::vector<double>& l,
	    const std::vector<double>& u,
	    const std::vector<double>& s) : lb(l),
					    ub(u),
					    stp(s),
					    prod(l.size()),
					    ub_true(l.size()),
					    card_each(l.size()) {
    card_ = 1;
    for (int i=0; i<l.size(); i++) {
      prod[i] = card_;
      card_each[i] = card_single(lb[i], ub[i], stp[i], ub_true[i]);
      card_ *= card_each[i]; 
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

  int round(array& a) const {
    int idx = 0;
    for (int i=0; i<a.size(); i++) {
      int tmpidx = std::round((a[i]-lb[i])/stp[i]);
      double tmpval = lb[i] + tmpidx*stp[i];
      if (tmpval > ub[i]) {
	std::cout << "warning: > ub" << std::endl;
	a[i] = ub_true[i];
	idx += prod[i]*card_each[i];
      } else if (tmpval < lb[i]) {
	std::cout << "warning: < lb" << std::endl;
	a[i] = lb[i];
      } else {
	a[i] = tmpval;
	idx += prod[i]*tmpidx;
      }
    }
    a.set_index(idx);
    return idx;
  }
};

#endif /* FUNCARRY_H */
