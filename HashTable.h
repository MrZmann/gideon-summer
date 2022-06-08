#pragma once

#include <iostream>
#include <cstdint>
#include <limits>

template<typename Key, typename Value>
class HashTable {
public:
        HashTable() = default;

        HashTable(double load) {
            std::cerr << "Unable to construct with set Load Factor for this graph type\n";
        }

        virtual ~HashTable() = default;
        
        virtual Value get(Key k) = 0;

        virtual void put(Key k, Value v) = 0;

        virtual void remove(Key k) = 0;

        virtual void display() = 0;

        virtual void stats() {
            std::cerr << "Unable to get Stats for this graph type\n";
        }

        virtual double getLoadFactor(){
            std::cerr << "Unable to get Load Factor for this graph type\n";
            return -1;
        }

        //return the full size of the hash table (including empty spots in the array)
        virtual uint64_t getFullSize(){
            std::cerr << "Unable to get Full Size for this graph type\n";
            return 0;
        }

        //return the number of Key Value Pairs in the hashtable
        virtual uint64_t getNumPairs() = 0;

};
