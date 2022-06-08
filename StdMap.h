#pragma once

#include "HashTable.h"
#include "Product.h"
#include <iostream>
#include <map>

template<typename Key, typename Value>
class StdMap : public HashTable<Key, Value> {
private:
    std::map<Key, Value> hashmap_;
public:
    StdMap() : hashmap_() {}

    StdMap(double load) : hashmap_() {
        std::cerr << "Ignoring load vector for STD Map\n";
    }

    Value get(Key k) override {
        return hashmap_[k];
    }

    void put(Key k, Value v) override {
        hashmap_[k] = v;
    }

    void remove(Key k) override {
        hashmap_.erase(k);
    }

    void display() override {
            for (auto kvp: hashmap_) {
                std::cout << " <" << kvp.first << ", " << kvp.second << "> \n";
            }
    }

    uint64_t getNumPairs() override {
        return hashmap_.size();
    }
};
