#pragma once

#include "product.h"
#include <list> 
#include <vector>

//implement hash map using a vector of linked lists for collisions
//use std::hash for string or any whatever w/ mod
//std::hash for int or random math w/ mod

//fill out report
//- resizing and when to resize
//- measuring collisions

#define hashmapSize 4

class StringToIntMap {
private:
    std::vector<std::list<std::pair<std::string, uint32_t>>> hashmap;

    std::hash<std::string> hasher;

    uint32_t hash(std::string x);

public:
    StringToIntMap() : hashmap(hashmapSize) {}

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

public:
    IntToProductMap() : hashmap(hashmapSize) {}

    Product* get(uint32_t s);

    void put(uint32_t i, Product* p);

    void remove(uint32_t t);

    void display();
};
