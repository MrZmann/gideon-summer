#pragma once

#include "HashTable.h"
#include "Product.h"
#include <iostream>
#include <unordered_map>

template<typename Key, typename Value>
class StdUnorderedMap : public HashTable<Key, Value> {
private:
    std::unordered_map<Key, Value> hashmap_;
public:
    StdUnorderedMap() : hashmap_() {}

    StdUnorderedMap(double load) : hashmap_() {
        std::cerr << "Ignoring load vector for STD Unordered Map\n";
    }

    Value get(const Key& k) override {
        return hashmap_[k];
    }

    void put(Key k, Value v) override {
        hashmap_[k] = v;
    }

    void remove(const Key& k) override {
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
