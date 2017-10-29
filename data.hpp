#include <iostream>
#include <cmath>
#include <numeric>

// basic data structure
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



struct func_arry {
  // parameters
private:
  std::vector<double> lb;
  std::vector<double> ub;
  std::vector<double> stp;

  array end_;

  int card_;

  int card_single(double l, const double& u, const double& s) {
    int ret = 0;
    while(l <= u) {
      ret++;
      l += s;
    };
    return ret;
  };
    
public:
  func_arry(const std::vector<double>& l,
	    const std::vector<double>& u,
	    const std::vector<double>& s) {
    lb   = l;
    ub   = u;
    stp  = s;
	
    card_ = 1;
    for (int i=0; i<l.size(); i++) {
      card_ *= card_single(lb[i], ub[i], stp[i]);
    }
  };
    
  const array& end() const {
    return end_;
  };

  array begin() const {
    return array(lb);
  };
    
  void inc(array& a) const {
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

  void reset(array& a) const {
    a.reset(lb);
  };


  int card() const {
    return card_;
  };
};

struct func_prof {
private:   
  std::vector<func_arry> arryfunc;
  const profile end_;
  int card_;
  
  // Denote the cardinality of the arryfuncs as c1, c2, ..., cn,
  // and the current idx of each arryfunc is i1, i2, ..., in.
  // Then the formula of index is,
  // index = i1*c2*c3*...*cn + ... + in-1*cn+in
    
  std::vector<int> skip_inc;
  std::vector<int> prod;
    
public:
  func_prof(const std::vector<func_arry>& af) : arryfunc(af),
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
    
  const profile& end() {
    return end_;
  };

  int card() {
    return card_;
  };
  
  profile begin() {
    profile ret;
    for(int i=0; i<arryfunc.size(); i++)
      ret.push_back(arryfunc[i].begin());
    ret.set_index(0);
    return ret;
  };

  void inc(profile& p) const {
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

  void inc_skip(profile& p, const int& omit) const {
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

  void inc_only(profile& p, const int& i) const {
    arryfunc[i].inc(p[i]);
    if (p[i] == arryfunc[i].end()) {
      p = end_;
      p.set_index(-1);
      return;
    }
    p.set_index(p.index() + prod[i]);
  }

  void reset(profile& p, const int& i) const {
    p[i] = arryfunc[i].begin();
    if (i>0)
      p.set_index(p.index() - (p.index()%prod[i-1] - p.index()%prod[i]));
    else
      p.set_index(p.index() - p.index()/prod[i]*prod[i]);
  }

  profile next(const profile& p) const {
    profile ret = p;
    inc(ret);
    return ret;
  }
};

struct model {
  std::vector<double> beta;
  profile get_next_state(const profile& p, const profile& a) {
    return profile();
  }
  std::vector<double> get_profit(const profile& p, const profile& a) {
    return std::vector<double>();
  }
};

struct alg {
  func_prof func_state;
  func_prof func_action;

  int m;
  int n;

  std::vector<int> min_b_idx;
    
  model config;

  std::vector<double> R;
    
  alg(const func_prof& s, const func_prof& a) : func_state(s), func_action(a) {
    init_R();
  };

  void init_R() {
    if (m!=4 or n!=2) {
      std::cerr<< "only support m=4 and n=2" << endl;
    }
    R = { -1,0,
	  1,0,
	  0,1,
	  0,-1 };
    min_b_idx = {0, 3}; // MAYBE WRONG
  };
  
  // set the first part of A matrix of constraint
  void set_A_1(std::vector<double>& A) {
    // The first part is just the R matrix
    for (int i=0; i<R.size(); i++) {
      A[i] = R[i];
    }
  };

  // the second part of A
  void set_A_2(std::vector<double>& A) {
    // The second part of A is even easier
    int idx = R.size();
    for (int i=0; i<n; i++) { // for each agent
      A[idx] = -config.beta[i];
      idx += n+1;
    }
  };

  // set the first part of b
  void set_b_1(std::vector<double>& b, const profile& state_prof,
	       const profile& action_prof, const std::vector<double>& W) {
    // figure out the next state profile
    profile next_state = config.get_next_state(state_prof, action_prof);
    for (int i=0; i<m; i++) {
      b[i] = W[next_state.index()*m+i];
    }
  }

  // set the second part of b
  void set_b_2(std::vector<double>& b, const profile& state_prof,
	       const profile& action_prof, const std::vector<double>& W) {
    // get current profit
    std::vector<double> crnt_profit = config.get_profit(state_prof, action_prof);
	
    profile iter;
    for (int i=0; i<n; i++) { // for each agent
      iter = action_prof;
      func_action.reset(iter, i);
      double minvalue = std::numeric_limits<double>::max();
      for ( ; iter!=func_action.end(); func_action.inc_only(iter,i) ) {
	std::vector<double> iter_profit = config.get_profit(state_prof, iter);
	profile iter_nxtstat = config.get_next_state(state_prof, iter);
	double min_b = W[iter_nxtstat.index()*m + min_b_idx[i]];
	double tmp = (1-config.beta[i])*(crnt_profit[i]-iter_profit[i]) - config.beta[i]*min_b;
	if (minvalue > tmp) {
	  tmp = minvalue;
	}
      }
      b[i] = minvalue;
    }
  };

  void linprog(const int&n, const int&m, const double* r,
	       const double* A, const double* b, const double& f,
	       const int &status) {
    if (m!=4 || n!=2) {
      std::cerr<< "only support m=4 and n=2" << endl;
    }
    // check feasibility
    if (b[1] < b[4]/A[8] || b[2] < b[5]/A[10]) {
      status = -1;
      f = std::numeric_limits<double>::min();
      return;
    }
    
    f = 0;
    for (int i=0; i<n; i++) {
      if (r[i] > 0) {
	f += r[i]*b[i+1]; // b's upper bound
      } else {
	f += r[i]*std::max(-b[3*i], b[m+i]/A[R.size()+i*n+i]); // b's lower bound
      }
    }
  };
  
  void solve() {
    std::vector<double> W(func_state.card()*m, 5);
    std::vector<double> W_new(W);

    profile action_prof = func_action.begin();
    profile state_prof = func_state.begin();
	
    while (true) {
      for ( ; state_prof != func_state.end(); func_state.inc(state_prof)) {
	for (int i = 0; i<m ; i++) {
	  // For each state and normal, we need to update
	  // the correponding constant and store it to
	  // wks, a per-thread vector to avoid false sharing
	  std::vector<double> wks(func_action.card());
	  // the constraints
	  std::vector<double> A((n + m)*n, 0.);
	  std::vector<double> b(n + m);
	  // set the first part of A
	  set_A_1(A);
	  // set the second part of A
	  set_A_2(A);

	  int idx = 0;
	  for ( ; action_prof != func_action.end(); func_action.inc(action_prof)) {
	    // generate first part of b
	    set_b_1(b, state_prof, action_prof, W);
	    // generate second part of b
	    set_b_2(b, state_prof, action_prof, W);
			
	    // do the linear programming
	    double f;
	    linprog(n, m, r, A, b, f);
	    
	    // store optimal value to wks
	    wks[i] = f;
	  }
	  // find the maximum of wks
	  W_new[state_prof.index()*m+i] = *std::max(wks.begin(), wks.end());
	} // for i
      }  // for state_prof

      // test convergence
      double normdiff = 0;
      for (int convidx = 0; convidx < W.size(); convidx++) {
	normdiff += std::pow(W[convidx] - W_new[convidx],2);
      }
      normdiff /= W.size();

      if (normdiff < 0.1) {
	// converged
      } else {
	W = W_new;
	continue;
      }
    }
  };
};
