#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <iostream>

struct array {
private:
  std::vector<double> value;
  int index_;
public:
  array();
  array(const std::vector<double>& v);
  array(const array& v) = default;
  std::vector<double>::size_type size() const;
  void reset(const std::vector<double>& v);
  bool operator!=(const array &b) const;
  bool operator==(const array &b) const;
  double& operator[](const int i);
  int operator++();
  int operator++(int);
  int index() const;
  void set_index(const int &i);
};

namespace std {
  ostream& operator<< (ostream& o, ::array& a);
}

#endif /* ARRAY_H */
