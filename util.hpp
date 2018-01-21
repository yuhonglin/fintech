#ifndef _UTIL_H_
#define _UTIL_H_

#include <stack>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

class util {
public:
  // profiling
  static std::stack<std::clock_t> _bnc_prof_tstart;
  static void tic(std::string msg = "tic");
  static void toc(std::string msg = "toc");
  
  // file system
  static std::vector<std::string> glob(const std::string& pat);

  // string
  template<typename Out>
  static void split(const std::string &s, char delim, Out result);
  static std::vector<std::string> split(const std::string &s, char delim);

  // io
  static void dump(std::vector<double>& v, const std::string& s);
  static void dump(             double* v, int l, const std::string& s);
  
};  // class util

#endif /* _UTIL_H_ */
