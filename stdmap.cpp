#include "stdmap.h"


uint32_t StringToIntMap::get(std::string s) {
    return hashmap[s];
}

void StringToIntMap::put(std::string s, uint32_t v) {
    hashmap[s] = v;
}

void StringToIntMap::remove(std::string s) {
    hashmap.erase(s);
}


Product* IntToProductMap::get(uint32_t s) {
    return hashmap[s];
}

void IntToProductMap::put(uint32_t i, Product* p) {
    hashmap[i] = p;
}

void IntToProductMap::remove(uint32_t t) {
    hashmap.erase(t);
}
