CXX=g++
CCFLAGS=-Wall -Werror -O3 -g -std=c++17

all:
	$(CXX) $(CCFLAGS) test.cc -o test 
