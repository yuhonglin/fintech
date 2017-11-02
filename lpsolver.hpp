#include <vector>
#include <memory>

extern "C" {
  void lpsol_  (int* mm, int* n,
		int* nclin, int* ldA, int* ldR,
		double* A, double* bl, double* bu, double* cvec,
		int* istate, int* kx, double* x, double* R, double* b,
		int* inform, int* iter, double* obj, double* clamda,
		int* iw, int* leniw, double* w, int* lenw, int* code );

};


class lp_solver {
private:
  // this class will hold the working arrays
  std::unique_ptr<double[]> w;
  std::unique_ptr<int[]>   iw;
  std::unique_ptr<double[]> clamda;
  std::unique_ptr<int[]> istate;
  std::unique_ptr<int[]> kx;
  std::unique_ptr<double[]> R;
  std::unique_ptr<double[]> b;  
  int n;
  int nclin;
  int m;
  int iter;
  int leniw;
  int lenw;
public:
  enum STATUS {
    SUCCESS,
    INFEASIBLE,
    ERROR
  };
  // n: size of the problem
  // m: number of inequality constraints
  lp_solver(int, int);

  void solve(double* c, double* A, double* lb, double* ub, int cold, // inputs
	     double& obj, double* sol, STATUS& stat); // outputs
};


