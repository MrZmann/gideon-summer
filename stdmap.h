#pragma once

#include "product.h"
#include <map>


class StringToIntMap {
    private:
        std::map<std::string, uint32_t> hashmap;
    public:
        StringToIntMap() : hashmap() {}

        uint32_t get(std::string s);

        void put(std::string s, uint32_t v);

        void remove(std::string s);

};

class IntToProductMap {
private:
    std::map<uint32_t, Product*> hashmap;

public:
    IntToProductMap() : hashmap() {}

    Product* get(uint32_t s);

    void put(uint32_t i, Product* p);

    void remove(uint32_t t);

};


