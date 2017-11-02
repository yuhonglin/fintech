#include "profile.hpp"


profile::profile() : index_(-1), value(0) {};
std::vector<double>::size_type profile::size() { return value.size(); };

array& profile::operator[](const int i) {
  return value[i];
};

bool profile::operator!=(const profile &b) const {
  return (index_!=b.index_) || (value!=b.value);
};

bool profile::operator==(const profile &b) const {
  return (index_==b.index_) && (value==b.value);
};

int profile::operator++() {
  return index_++;
};

int profile::operator++(int) {
  return ++index_;
};

void profile::set_index(const int &i) { index_ = i; }

void profile::push_back(const array &a) { value.push_back(a); }

int profile::index() { return index_; };

namespace std {
  ostream& operator<< (ostream& o, ::profile& p) {
    o << '(' << p.index() << ")\t";	
    for (int i=0; i<p.size(); i++) {
      for (int j=0; j<p[i].size(); j++) {
	o << p[i][j] << ',';
      };
      cout << " | ";
    };
    return o;
  };
};

