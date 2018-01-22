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
  
  int card_single(double l, const double& u, const double& s, double& ut);
    
public:
  func_arry(const std::vector<double>& l,
	    const std::vector<double>& u,
	    const std::vector<double>& s);
    
  const array& end() const;

  array begin() const;
    
  void inc(array& a) const;

  void reset(array& a) const;

  int card() const;

  int update_index(array& a) const;
  
  int round(array& a) const;

  void set_ub(const double& u, const int& i);

  void set_lb(const double& l, const int& i);

  bool operator== (const func_arry& fa) const;
  bool operator!= (const func_arry& fa) const;
};

#endif /* FUNCARRY_H */
