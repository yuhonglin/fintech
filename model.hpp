#ifndef MODEL_H
#define MODEL_H

struct model {
  const std::vector<double> beta;
  std::vector<double> beta;
  profile get_next_state(const profile& p, const profile& a) {
    const double d1 = 0;
    return profile();
  }
  std::vector<double> get_profit(const profile& p, const profile& a) {
    return std::vector<double>();
  }
};


#endif /* MODEL_H */
