#include "funcarry.hpp"

  
int func_arry::card_single(double l, const double& u, const double& s, double& ut) {
  int ret = 0;
  while(l <= u) {
    ut = l;
    ret++;
    l += s;
  };
  
  return ret;
};
    
func_arry::func_arry(const std::vector<double>& l,
		    const std::vector<double>& u,
		    const std::vector<double>& s) : lb(l),
						    ub(u),
						    stp(s),
						    prod(l.size()),
						    ub_true(l.size()),
						    card_each(l.size()) {
  card_ = 1;
  for (int i=l.size()-1; i>=0; i--) {
    prod[i] = card_;
    card_each[i] = card_single(lb[i], ub[i], stp[i], ub_true[i]);
    card_ *= card_each[i]; 
  }
};
    
const array& func_arry::end() const {
  return end_;
};

array func_arry::begin() const {
  return array(lb);
};
    
void func_arry::inc(array& a) const {
  for (int i=a.size()-1; i>=0; i--) {
    if (a[i] + stp[i] > ub[i]) {
      a[i] = lb[i];
      continue;
    } else {
      a[i] += stp[i];
      a++;
      return;
    }
  }
  a = end();
  return;
};

void func_arry::reset(array& a) const {
  a.reset(lb);
};

int func_arry::card() const {
  return card_;
};

void func_arry::set_ub(const double& u, const int& i) {
  ub[i] = u;

  int tmp = card_single(lb[i], ub[i], stp[i], ub_true[i]);
  // update card_
  card_ = card_ / card_each[i] * tmp;
  // update prod
  for (int j=0; j<i; j++) {
    prod[j] = prod[j] / card_each[i] * tmp;
  }
  // update card_each
  card_each[i] = tmp;
}

void func_arry::set_lb(const double& l, const int& i) {
  lb[i] = l;

  int tmp = card_single(lb[i], ub[i], stp[i], ub_true[i]);
  // update card_
  card_ = card_ / card_each[i] * tmp;
  // update prod
  for (int j=0; j<i; j++) {
    prod[j] = prod[j] / card_each[i] * tmp;
  }
  // update card_each
  card_each[i] = tmp;
}


int func_arry::round(array& a) const {
  int idx = 0;
  for (int i=0; i<a.size(); i++) {
    if (a[i] < lb[i]) {
      std::cout << "warning: < lb : " << a[i] << std::endl;
    }
    if (a[i] > ub_true[i]) {
      std::cout << "warning: > ub : " << a[i] << std::endl;
    }
    int tmpidx = std::round((a[i]-lb[i])/stp[i]);
    
    double tmpval = lb[i] + tmpidx*stp[i];
    if (tmpval > ub[i]) {
      a[i] = ub_true[i];
      idx += prod[i]*card_each[i];
    } else if (tmpval < lb[i]) {
      a[i] = lb[i];
    } else {
      a[i] = tmpval;
      idx += prod[i]*tmpidx;
    }
  }
  a.set_index(idx);
  return idx;
}


bool func_arry::operator==(const func_arry& fa) const {
  return (lb==fa.lb) && (ub==fa.ub) && (stp==fa.stp) && (ub_true==fa.ub_true)
    && (end_==fa.end_)
    && (card_==fa.card_) && (prod==fa.prod) && (card_each==fa.card_each);
}


bool func_arry::operator!= (const func_arry& fa) const {
  return !(*this==fa);
}
