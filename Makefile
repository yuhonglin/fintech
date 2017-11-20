fortran_obj = $(patsubst %.f,%.o,$(wildcard *.f))
cpp_obj = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

option = -O3 -lm -fopenmp

%.o: %.f 
	gfortran $(option) -c $^ -o $@

%.o: %.cpp 
	g++ $(option) -std=c++11 -c $^ -o $@

run: $(fortran_obj) $(cpp_obj) 
	g++ $(option) -std=c++11 $^ -o $@ -lgfortran

clean:
	rm *.o
	rm ./run
