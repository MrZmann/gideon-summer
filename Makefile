std:
	g++ test.cc stdmap.cpp -o test -DUSE_STD_MAP
chained:
	g++ test.cc chained.cpp -o test -DUSE_CHAINED_MAP
