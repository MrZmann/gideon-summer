#pragma once

#include "product.h"
#include "stats.h"
#include <iostream>
#include <list> 
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

#define MAX_LOAD_FACTOR 0.75

class StringToIntMap {
private:
    std::vector<std::list<std::pair<std::string, uint32_t>>> hashmap;

    std::hash<std::string> hasher;

    uint32_t hash(std::string x);

    uint32_t hashmapSize = 4;

    uint32_t numPairs = 0;

    void doubleSize();

public:
    StringToIntMap() : hashmap(4) {}

    StringToIntMap(uint32_t s) : hashmap(s) {hashmapSize = s;}

    uint32_t get(std::string s);

    void put(std::string s, uint32_t v);

    void remove(std::string s);

    void display();

};

class IntToProductMap {
private:
    std::vector<std::list<std::pair<uint32_t, Product*>>> hashmap;

    std::hash<uint32_t> hasher;

    uint32_t hash(uint32_t x);

    uint32_t hashmapSize = 4;

    uint32_t numPairs = 0;

    void doubleSize();

public:
    IntToProductMap() : hashmap(4) {}

    IntToProductMap(uint32_t s) : hashmap(s) {hashmapSize = s;}

    Product* get(uint32_t s);

    void put(uint32_t i, Product* p);

    void remove(uint32_t t);

    void display();
};
