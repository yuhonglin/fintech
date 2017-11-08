#include "funcprof.hpp"

func_prof::func_prof(const func_prof& fp) {
  arryfunc = fp.arryfunc;
  end_ = fp.end_;
  card_ = fp.card_;
  skip_inc = fp.skip_inc;
  prod = fp.prod;
};
  
func_prof::func_prof(const std::vector<func_arry>& af) : arryfunc(af),
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
    
const profile& func_prof::end() {
  return end_;
};

int func_prof::card() {
  return card_;
};
  
profile func_prof::begin() {
  profile ret;
  for(int i=0; i<arryfunc.size(); i++)
    ret.push_back(arryfunc[i].begin());
  ret.set_index(0);
  return ret;
};

void func_prof::inc(profile& p) const {
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

void func_prof::inc_skip(profile& p, const int& omit) const {
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

void func_prof::inc_only(profile& p, const int& i) const {
  arryfunc[i].inc(p[i]);
  if (p[i] == arryfunc[i].end()) {
    p = end_;
    p.set_index(-1);
    return;
  }
  p.set_index(p.index() + prod[i]);
}

void func_prof::reset(profile& p, const int& i) const {
  p[i] = arryfunc[i].begin();
  if (i>0)
    p.set_index(p.index() - (p.index()%prod[i-1] - p.index()%prod[i]));
  else
    p.set_index(p.index() - p.index()/prod[i]*prod[i]);
}

profile func_prof::next(const profile& p) const {
  profile ret = p;
  inc(ret);
  return ret;
}

func_arry& func_prof::operator[] (const int &i) {
  return arryfunc[i];
}

void func_prof::set_ub(const std::vector<double>&ub, const int& idx) {
  // First, update the arryfunc
  for (int i=0; i<arryfunc.size(); i++) {
    arryfunc[i].set_ub(ub[i], idx);
  }
  // Then repeat the code in constructor
  skip_inc.back() = arryfunc.back().card()-1;
  prod.back() = 1;
  for (int i=arryfunc.size()-2; i>=0; i--) {
    prod[i] = prod[i+1]*arryfunc[i+1].card();
    // Let me explain the following equation, the index increment of 
    // "incmenting with skipping" at i can be computed in two steps
    // 1. increase the index by the total cardinality of i to 1.
    // 2. decrease the index by the total cardinality of i-1 to 1 minusing 1.
    // So the result is prod[i-1]*card[i] - (prod[i-1]-1)
    // Because the algorithm will increment the index by 1 later, still
    // need to remove one from the above.
    skip_inc[i] = prod[i]*(arryfunc[i].card()-1);
  }
  card_ = prod[0]*arryfunc[0].card();
}

int func_prof::round(profile& p) const {
  //  std::cout << "----------------------" << std::endl;
  //  std::cout << p << std::endl;
  int idx = 0;
  for (int i=0; i < p.size(); i++) {
    idx += arryfunc[i].round(p[i])*prod[i];
  }
  p.set_index(idx);
  //  std::cout << p << std::endl;  
  return idx;
}
