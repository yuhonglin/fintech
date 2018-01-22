#include <limits>
#include <utility>

#include "model.hpp"

const int& model::num_agent() const {
  return num_agent_;
}

const int& model::num_normal() const {
  return num_normal_;
}

const double& model::init_constant() const {
  return init_constant_;
}

const std::vector<double>& model::beta() const {
  return beta_;
}

model& model::set_num_agent(const int& i) {
  num_agent_ = i;
  return *this;
}

model& model::set_num_normal(const int& i) {
  num_normal_ = i;
  return *this;
}

model& model::set_beta(const std::vector<double>& b) {
  beta_ = b;
  return *this;
}

model& model::set_init_constant(const double& c) {
  init_constant_ = c;
  return *this;
}

model& model::set_func_state(const func_prof& fs) {
  func_state = fs;
  return *this;
}

model& model::set_func_action(const func_prof& fa) {
  func_action = fa;
  return *this;
}

std::pair<std::vector<double>, std::vector<double>>
model::stage_profit_bound() {
  std::pair<std::vector<double>, std::vector<double>>
    ret(std::vector<double>(num_agent_, -std::numeric_limits<double>::infinity()),
	std::vector<double>(num_agent_,  std::numeric_limits<double>::infinity()));
  auto func_state = get_state_func();
  for (profile sp = func_state.begin(); sp != func_state.end(); func_state.inc(sp)) {
    func_prof func_action = get_action_func(sp);
    for (profile ap = func_action.begin(); ap != func_state.end(); func_action.inc(ap)) {
      auto prft = get_profit(sp, ap);
      for (int i = 0; i < num_agent_; i++) {
	if (prft[i] > ret.first[i])  ret.first[i] = prft[i];
	if (prft[i] < ret.second[i]) ret.second[i] = prft[i];
      }
    }    
  }
  return ret;
}

bool model::if_skip(const profile &sp, const profile &act) {
  return false;
}
