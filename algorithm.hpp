#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <iostream>
#include <numerics>
#include <cmath>

#include "profile.hpp"
#include "model.hpp"
#include "workset.hpp"

using ActionProfile = Profile<Action>;
using StateProfile = Profile<State>;

class Algorithm {

    const int batch   = 100;
    const int nthread =  10;

    int m = 12;

    Algorithm() {
    }
    
    std::unique_ptr<double> R;

    void init_normal() {
	if (m!=2) {
	    std::cerr << "Currently only support m=2" << std::endl;
	    exit(1);
	}
	
	R.reset(new double[ActionProfile::num_agent*m]);
	
	for (int i = 0; i < m;) {
	    R[i++] = std::cos(i*3.14159265359*2/m);
	    R[i++] = std::cos(i*3.14159265359*2/m);
	}
    }
    
    void solve() {
	StateProfile statprof;
	ActionProfile actprof;
	Workset<StateProfile> W;
	Workset<StateProfile> W_new;
	while (true) {
	    for ( ; statprof!=StateProfile::end; ++statprof) {
		for (int i = 0; i<m; i++) {
		    // For each state and normal, we need to update
		    // the correponding constant and store it to
		    // W_new[statprof.index+i]
		    std::vector<double> wks(ActProfile::get_card(),
					    std::numeric_limits<double>::quiet_NaN());
		    // allocate memory for constraints
		    
		    for ( ; actprof!=ActProfile::end; ++actprof) {
			// next state profile
			StateProf nxtstatprof = Model::get_next_state(statprof, actprof);
			// current profit

			// generate constraint for firms
			
		    }
		}
	    }
	}
    };

    solve_(StateProf statprof, ActionProf actprof) {
	
    };
};

#endif /* ALGORITHM_H */
