#ifndef PROFILE_H
#define PROFILE_H

#include <numeric>
#include <iostream>
#include <cmath>
#include <vector>

#include "array.hpp"

struct profile
{
private:
  std::vector<array> value;
  int index_;
public:
  profile();
  std::vector<double>::size_type size();
  array& operator[](const int i);
  bool operator!=(const profile &b) const;
  bool operator==(const profile &b) const;
  int operator++();
  int operator++(int);
  void set_index(const int &i);
  void push_back(const array &a);
  int index();
};

namespace std {
  ostream& operator<< (ostream& o, ::profile& p);
}


#endif /* PROFILE_H */
