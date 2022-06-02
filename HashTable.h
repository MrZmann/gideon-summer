#pragma once

#include <cstdint>
#include <limits>

template<typename Key, typename Value>
class HashTable {
public:
        HashTable() = default;

        ~HashTable() = default;
        
        virtual Value get(Key k) = 0;

        virtual void put(Key k, Value v) = 0;

        virtual void remove(Key k) = 0;
};