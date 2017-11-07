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
