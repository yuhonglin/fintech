#include "array.hpp"

array::array() : index_(-1), value(0) {};
array::array(const std::vector<double>& v) : index_(0), value(v) {};

std::vector<double>::size_type array::size() const { return value.size(); };

void array::reset(const std::vector<double>& v) { value=v; index_=0; };

bool array::operator!=(const array &b) const {
  return (index_!=b.index_) || (value!=b.value);
};

bool array::operator==(const array &b) const {
  return (index_==b.index_) && (value==b.value);
};

double& array::operator[](const int i) {
  return value[i];
};

int array::operator++() {
  return index_++;
};

int array::operator++(int) {
  return ++index_;
};

int array::index() const {
  return index_;
};

void array::set_index(const int &i) {
  index_   = i;
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
