#include "product.h"
#include <list> 
#include <vector>

#ifndef CHAINED_MAP
#define CHAINED_MAP
//implement hash map using a vector of linked lists for collisions
//use std::hash for string or any whatever w/ mod
//std::hash for int or random math w/ mod

//fill out report
//- resizing and when to resize
//- measuring collisions
class StringToIntMap {
    public:
    StringToIntMap() {}

    uint32_t get(std::string s) { return 0; }

    void put(std::string s, uint32_t v) {}

    void remove(std::string s) {}

};

class IntToProductMap {
    public:
    IntToProductMap() {}

    Product* get(uint32_t s) { return nullptr; }

    void put(uint32_t i, Product* p) {}

    void remove(uint32_t t) {}

};


#endif
