#ifndef FUNCTOR_H
#define FUNCTOR_H

#include <numeric>
#include <iostream>
#include <cmath>
#include <vector>

#include "profile.hpp"
#include "funcarry.hpp"


struct func_prof {
private:   
  std::vector<func_arry> arryfunc;
  profile end_;
  int card_;
  
  // Denote the cardinality of the arryfuncs as c1, c2, ..., cn,
  // and the current idx of each arryfunc is i1, i2, ..., in.
  // Then the formula of index is,
  // index = i1*c2*c3*...*cn + ... + in-1*cn+in
    
  std::vector<int> skip_inc;
  std::vector<int> prod;
    
public:
  func_prof() = default;
  func_prof(const func_prof& fp) {
    arryfunc = fp.arryfunc;
    end_ = fp.end_;
    card_ = fp.card_;
    skip_inc = fp.skip_inc;
    prod = fp.prod;
  };
  
  func_prof(const std::vector<func_arry>& af) : arryfunc(af),
						skip_inc(af.size()),
						prod(af.size()) {
    skip_inc.back() = af.back().card()-1;
    prod.back() = 1;
    for (int i=af.size()-2; i>=0; i--) {
      prod[i] = prod[i+1]*af[i+1].card();
      // Let me explain the following equation, the index increment of 
      // "incmenting with skipping" at i can be computed in two steps
      // 1. increase the index by the total cardinality of i to 1.
      // 2. decrease the index by the total cardinality of i-1 to 1 minusing 1.
      // So the result is prod[i-1]*card[i] - (prod[i-1]-1)
      // Because the algorithm will increment the index by 1 later, still
      // need to remove one from the above.
      skip_inc[i] = prod[i]*(af[i].card()-1);
    }
    card_ = prod[0]*af[0].card();
  };
    
  const profile& end() {
    return end_;
  };

  int card() {
    return card_;
  };
  
  profile begin() {
    profile ret;
    for(int i=0; i<arryfunc.size(); i++)
      ret.push_back(arryfunc[i].begin());
    ret.set_index(0);
    return ret;
  };

  void inc(profile& p) const {
    for (int i=p.size()-1; i>=0; i--) {
      arryfunc[i].inc(p[i]);
      if (p[i] == arryfunc[i].end()) {
	arryfunc[i].reset(p[i]);
	continue;
      } else {
	p++;
	return;
      }
    }
    p = end_;
    p.set_index(-1);
    return;
  };

  void inc_skip(profile& p, const int& omit) const {
    for (int i=p.size()-1; i>=0; i--) {
      if (i == omit) {
	p.set_index(p.index()+skip_inc[i]);
	continue;
      }
      arryfunc[i].inc(p[i]);
      if (p[i] == arryfunc[i].end()) {
	arryfunc[i].reset(p[i]);
	continue;
      } else {
	p++;
	return;
      }
    }
    p = end_;
    p.set_index(-1);
    return;
  };

  void inc_only(profile& p, const int& i) const {
    arryfunc[i].inc(p[i]);
    if (p[i] == arryfunc[i].end()) {
      p = end_;
      p.set_index(-1);
      return;
    }
    p.set_index(p.index() + prod[i]);
  }

  void reset(profile& p, const int& i) const {
    p[i] = arryfunc[i].begin();
    if (i>0)
      p.set_index(p.index() - (p.index()%prod[i-1] - p.index()%prod[i]));
    else
      p.set_index(p.index() - p.index()/prod[i]*prod[i]);
  }

  profile next(const profile& p) const {
    profile ret = p;
    inc(ret);
    return ret;
  }
};

#endif /* FUNCTOR_H */
