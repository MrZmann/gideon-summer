#pragma once

#include <iostream>
#include <cstdint>
#include <limits>

template<typename Key, typename Value>
class HashTable {
public:
        HashTable() = default;

        HashTable(double load) {
            std::cerr << "Unable to set Load Factor for this graph type";
        }

        virtual ~HashTable() = default;
        
        virtual Value get(Key k) = 0;

        virtual void put(Key k, Value v) = 0;

        virtual void remove(Key k) = 0;

        virtual void display(bool showRawData) = 0;
};