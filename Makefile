fortran_obj = $(patsubst %.f,%.o,$(wildcard *.f))
cpp_obj = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

CXX = g++-6
FC  = gfortran-6

option = -O3 -fopenmp

%.o: %.f Makefile
	$(FC) $(option) -c $(word 1,$^) -o $@

%.o: %.cpp %.hpp Makefile
	$(CXX) $(option) -std=c++11 -c $(word 1,$^) -o $@

run: $(fortran_obj) $(cpp_obj)
	$(CXX) $(option) -std=c++11 $^ -o $@ -lgfortran

clean:
	rm *.o
	rm ./run
