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
  func_prof(const func_prof& fp);
  
  func_prof(const std::vector<func_arry>& af);
    
  const profile& end();

  int card();
  
  profile begin();

  void inc(profile& p) const;

  void inc_skip(profile& p, const int& omit) const;

  void inc_only(profile& p, const int& i) const;

  void reset(profile& p, const int& i) const;
  
  profile next(const profile& p) const;

  int round(profile& p) const;

  func_arry& operator[] (const int &i);

  void set_ub(const std::vector<double>&, const int&);
};

#endif /* FUNCTOR_H */
