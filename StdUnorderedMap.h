#pragma once

#include "HashTable.h"
#include "Product.h"
#include <iostream>
#include <unordered_map>

template<typename Key, typename Value>
class StdUnorderedMap : public HashTable<Key, Value> {
private:
    std::unordered_map<Key, Value> hashmap;
public:
    StdUnorderedMap() : hashmap() {}

    StdUnorderedMap(double load) : hashmap() {
        std::cerr << "Ignoring load vector for STD Unordered Map\n";
    }

    Value get(Key k) {
        return hashmap[k];
    }

    void put(Key k, Value v){
        hashmap[k] = v;
    }

    void remove(Key k){
        hashmap.erase(k);
    }

    void display(bool showRawData){
        if(showRawData) {
            for (auto kvp: hashmap) {
                std::cout << " <" << kvp.first << ", " << kvp.second << "> \n";
            }
        }
        std::cout << "Unable to generate stats for STD Unordered Map" << std::endl;
    }
};
