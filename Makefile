CXX=g++
CCFLAGS=-Wall -Werror -O3 -g -std=c++17

test:
	$(CXX) $(CCFLAGS) test.cc -o test
benchmark:
	$(CXX) $(CCFLAGS) benchmark.cc -o test
