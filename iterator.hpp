#ifndef ITERATOR_H
#define ITERATOR_H

#include <iostream>
#include <vector>

class IndexIterator
{
    
public:
    IndexIterator(std::vector<int> r) : ranges(r), iters(r.size(),0) {
	iters[iters.size()-1] = -1;
    }
    virtual ~IndexIterator() = default;

    std::vector<int>* next() {
	for (int i = ranges.size()-1; i >= 0; i--) {
	    if (iters[i] == ranges[i]-1) {
		iters[i] = 0;
		continue;
	    } else {
		++iters[i];
		return &iters;
	    }
	}
	return nullptr;
    }
    
private:
    std::vector<int> ranges;
    std::vector<int> iters;

};




#endif /* ITERATOR_H */
