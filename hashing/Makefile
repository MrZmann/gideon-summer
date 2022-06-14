CXX=g++
CCFLAGS=-Wall -Werror -O3 -g -std=c++17
CRITICAL=murmur3.c

test:
	$(CXX) $(CCFLAGS) test.cc $(CRITICAL) -o test
bench:
	$(CXX) $(CCFLAGS) benchmark.cc $(CRITICAL) -o bench
benchclean:
	$(CXX) $(CCFLAGS) benchmarkClean.cc $(CRITICAL) -o benchclean
