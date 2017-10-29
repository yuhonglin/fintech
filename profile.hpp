#ifndef PROFILE_H
#define PROFILE_H


struct profile
{
private:
  std::vector<array> value;
  int index_;
public:
  profile() : index_(-1), value(0) {};
  std::vector<double>::size_type size() { return value.size(); };
  array& operator[](const int i) {
    return value[i];
  };
  bool operator!=(const profile &b) const {
    return (index_!=b.index_) || (value!=b.value);
  };
  bool operator==(const profile &b) const {
    return (index_==b.index_) && (value==b.value);
  };
  int operator++() {
    return index_++;
  };
  int operator++(int) {
    return ++index_;
  };
  void set_index(const int &i) { index_ = i; }
  void push_back(const array &a) { value.push_back(a); }
  int index() { return index_; };
};


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


#endif /* PROFILE_H */
