#ifndef STATE_H
#define STATE_H

#include <vector>
#include <algorithm>
#include <numeric>
#include <math>

struct State {
    State() : value(lb), index(0) {};

    State(double v, int i) : value(lb.size(), v), index(i) {};
    
    const State& operator++ (int) {
	for (int i=value.size()-1; i>=0; i--) {
	    if (value[i] + stp[i] > ub[i]) {
		value[i] = lb[i];
		continue;
	    } else {
		value[i] += stp[i];
		index++;
		return *this;
	    }
	}
	(*this) = end;
	index = -1;
	return end;
    }

    void reset() {
	value = lb;
    };

    static int get_card() {
	int ret = 1;
	for (int i=0; i<lb.size(); i++) {
	    ret *= std::floor((ub[i]-lb[i])/stp[i]);
	}
	return ret;
    };

    static const std::vector<double> lb;
    static const std::vector<double> ub;
    static const std::vector<double> stp;

    static const State end;
  
    std::vector<double> value;
    int index;
};

const State State::end(std::numeric_limits<double>::quiet_NaN(), -1);

#endif /* STATE_H */
