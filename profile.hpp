#ifndef PROFILE_H
#define PROFILE_H

#include <vector>

template<class T>
struct Profile
{
public:
    Profile() : value(num_agent), index(0) {};
    Profile(const T& t, int i) : value(num_agent, t), index(i) {};
    
    const Profile& operator++ (int) {
	for (int i=value.size()-1; i>=0; i--) {
	    if ((++value[i]) == T::end) {
		value[i].reset();
		continue;
	    } else {
		index++;
		return *this;
	    }
	}
	(*this) = end;
	index = -1;
	return end;
    };

    const Profile& inc_omit(int idx) {
	for (int i=value.size()-1; i>=0; i--) {
	    if (i == idx) {
		index += T::get_card()*(value.size()-i) - 1;
		continue;
	    }
	    if ((++value[i]) == T::end) {
		value[i].reset();
		continue;
	    } else {
		++value[i];
		index++;
		return *this;
	    }
	}
	(*this) = end;
	index = -1;
	return end;
    };
    
    static int get_card() {
	int ret = 1;
	for (int i=0; i<num_agent; i++) {
	    ret *= T::get_card();
	}
	return ret;
    };

    std::vector<T> value;
    int index;
    
    static const Profile<T> end;
    static const int  num_agent; // number of agents
};

template<class T>
const Profile<T> Profile<T>::end(T::end, -1);

#endif /* PROFILE_H */
