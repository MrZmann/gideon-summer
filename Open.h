#pragma once

#include "HashTable.h"
#include "Product.h"
#include <iostream>
#include <map>

template<typename Key, typename Value>
class OpenMap: public HashTable<Key, Value> {
    public:
        OpenMap() = default;

        Value get(Key k) = 0;

        void put(Key k, Value v) = 0;

        void remove(Key k) = 0;
};
