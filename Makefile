fortran_obj = $(patsubst %.f,%.o,$(wildcard *.f))
cpp_obj = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

CXX = g++-5
FC  = gfortran-5

option = -O3 -lm -fopenmp

%.o: %.f 
	$(FC) $(option) -c $^ -o $@

%.o: %.cpp 
	$(CXX) $(option) -std=c++11 -c $^ -o $@

run: $(fortran_obj) $(cpp_obj) 
	$(CXX) $(option) -std=c++11 $^ -o $@ -lgfortran

clean:
	rm *.o
	rm ./run
