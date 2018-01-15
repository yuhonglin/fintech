#include <vector>
#include <sstream>

#include <glob.h>

#include "util.hpp"
  
std::stack<std::clock_t> util::_bnc_prof_tstart;

void util::tic(std::string msg) {
  std::cout << msg << std::endl;
  _bnc_prof_tstart.push(clock());
}

void util::toc(std::string msg) {
  std::clock_t tend = clock();
  std::cout << msg << ", costs cpu time: "
	    << (static_cast<double>(tend-_bnc_prof_tstart.top())/CLOCKS_PER_SEC)
	    << " second(s)." << std::endl;
  _bnc_prof_tstart.pop();
}
    

std::vector<std::string> util::glob(const std::string& pat) {
  using namespace std;
  glob_t glob_result;
  ::glob(pat.c_str(),GLOB_TILDE,NULL,&glob_result);
  vector<string> ret;
  for(unsigned int i=0;i<glob_result.gl_pathc;++i){
    ret.push_back(string(glob_result.gl_pathv[i]));
  }
  globfree(&glob_result);
  return ret;
}

template<typename Out>
void util::split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> util::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
