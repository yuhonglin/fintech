#ifndef WORKSET_H
#define WORKSET_H

#include <memory>

template<class SP>
struct Workset {

    // number of normal
    int num_normal;
    std::unique_ptr<double> data;
    
    Workset(const int& m) : data(new double[SP::get_card()*num_normal]),
			    num_normal(m) {};

    Workset(const Workset<SP>& w) {
	if (num_normal != w.num_normal) {
	    num_normal  = w.num_normal;
	    const int tmp = SP::get_card()*num_normal;
	    data.reset(new double[tmp]);
	    for (int i=0; i<tmp; i++) {
		data[i] = w.data[i];
	    }
	}
    };
};

#endif /* WORKSET_H */
