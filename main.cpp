#include <iostream>
#include <vector>

#include "data.hpp"


int main(int argc, char *argv[])
{
    func_arry act1 (
	{0.1, 0.2, 0.3},
	{0.5, 0.5, 0.8},
	{0.1, 0.2, 0.3}
	);

    func_arry act2 (
	{1, 2, 3},
	{5, 5, 8},
	{1, 2, 3}
	);

    func_arry act3 (
	{10, 20, 30},
	{50, 50, 80},
	{10, 20, 30}
	);

    
    func_prof prof (
	{act1, act2, act3}
	);

    profile p = prof.begin();

    for (int i=0; i < 30; i++)
      prof.inc(p);

    prof.reset(p, 0);
    
    std::cout << p << std::endl;
    
    //    while(p!=prof.end()) {
    //	std::cout << p << std::endl;
      //prof.inc_only(p,2);
	//prof.inc_skip(p,0);
	//prof.inc(p);
    //    }
}
