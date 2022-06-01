#pragma once

#include "HashTable.h"
#include "Product.h"
#include <iostream>
#include <map>

template<typename Key, typename Value>
class StdMap : public HashTable<Key, Value> {
    private:
        std::map<Key, Value> hashmap;
    public:
        StdMap() : hashmap() {}

        Value get(Key k) {
            return hashmap[k];
        }

        void put(Key k, Value v){
            hashmap[k] = v;
        }

        void remove(Key k){
            hashmap.erase(k);
        }

};
