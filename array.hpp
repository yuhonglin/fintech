#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <iostream>

struct array {
private:
  std::vector<double> value;
  int index_;
public:
  array() : index_(-1), value(0) {};
  array(const std::vector<double>& v) : index_(0), value(v) {};
  array(const array& v) = default;
  std::vector<double>::size_type size() const { return value.size(); };
  void reset(const std::vector<double>& v) { value=v; index_=0; };
  bool operator!=(const array &b) const {
    return (index_!=b.index_) || (value!=b.value);
  };
  bool operator==(const array &b) const {
    return (index_==b.index_) && (value==b.value);
  };
  double& operator[](const int i) {
    return value[i];
  };
  int operator++() {
    return index_++;
  };
  int operator++(int) {
    return ++index_;
  };
  int index() const {
    return index_;
  }
  int set_index(const int &i) {
    index_ = i;
  }
};

namespace std {
  ostream& operator<< (ostream& o, ::array& a) {
    o << '(' << a.index() << ")\t";
    for (int i=0; i<a.size(); i++) {
      o << a[i] << ',';
    };
    return o;
  };
};

#endif /* ARRAY_H */
