#pragma once

#include "HashTable.h"
#include "Product.h"
#include "stats.h"
#include "murmur3.h"

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>

#define DEFAULT_CHAINED_MAP_SIZE 4

template<typename Key, typename Value>
class ChainedMap : public HashTable<Key, Value> {
public:
    ChainedMap() {
        //hashmap_ is a vector of lists of KeyValuePairs (key, value)
        hashmap_.resize(DEFAULT_CHAINED_MAP_SIZE);
    }

    ChainedMap(double load) {
        hashmap_.resize(DEFAULT_CHAINED_MAP_SIZE);
        MAX_LOAD_FACTOR_ = load;
    }

    Value get(const Key& k) override {
        uint64_t index = hash(k, hashmap_.size());
        //if any key in the list at the index of the hashed key matches the key, return its value
        for(const auto& kvp : hashmap_[index]){
            if(k == kvp.key_){
                return kvp.value_;
            }
        }
        //otherwise return the default value
        return Value{};
    }

    void put(Key k, Value v) override {
        uint64_t index = hash(k, hashmap_.size());
        //if any key in the list at the index of the hashed key matches the key, change its value
        for(auto& kvp : hashmap_[index]){
            if(kvp.key_ == k){
                kvp.value_ = v;
                return;
            }
        }
        //otherwise push the KeyValuePair to the back of the list at the index of the hashed key
        KeyValuePair kvp(k, v);
        hashmap_[index].push_back(kvp);
        ++numPairs_;
        if(getLoadFactor() > MAX_LOAD_FACTOR_){
            doubleSize();
        }
    }

    void remove(const Key& k) override {
        uint64_t index = hash(k, hashmap_.size());
        //iterate through every spot in the list at the index of the hashed key, and if the keys match, erase the KeyValuePair
        for(auto i = hashmap_[index].begin(); i != hashmap_[index].end(); ++i) {
            if(k == i->key_){
                hashmap_[index].erase(i);
                --numPairs_;
                return;
            }
        }
        std::cerr << "Chained Map does not contain the key \"" << k << "\" so it could not be removed\n";
    }

    void display() override {
        //index: |number of elements| <key1, value1> <key2, value2> ...
        for(uint64_t i = 0; i < hashmap_.size(); i++){
            std::cout << i << ": |" << hashmap_[i].size() << "| ";
            for(const auto& kvp : hashmap_[i]){
                    printKeyValuePair(kvp);
            }
                std::cout << "\n";
        }
        std::cout << std::endl;
    }

    void stats() override {
        std::vector<uint64_t> elements;
        std::vector<uint64_t> collisions;
        uint64_t numWith0 = 0;
        uint64_t numWith1 = 0;
        uint64_t numWithMore = 0;

        for(uint64_t i = 0; i < hashmap_.size(); i++){
            elements.push_back(static_cast<uint64_t>(hashmap_[i].size()));
            if(hashmap_[i].empty()) {
                ++numWith0;
                collisions.push_back(0);
            } else {
                collisions.push_back(static_cast<uint64_t>(hashmap_[i].size()-1));
                if(hashmap_[i].size() == 1){
                    ++numWith1;
                }
                else{
                    ++numWithMore;
                }
            }
        }
        std::cout << std::endl;

        if(elements.empty()){
            std::cout << "No Elements\n";
        } else {
            std::sort(elements.begin(), elements.end());
            Statistic elementStats = evaluateStats(&elements);
            std::cout << "Total Elements: " << elementStats.total << "\n";
            std::cout << "Mean Elements: " << elementStats.mean << "\n";
            std::cout << "Stdev Elements: " << elementStats.stdev << "\n";

            std::sort(collisions.begin(), collisions.end());
            Statistic collisionStats = evaluateStats(&collisions);
            std::cout << "Total Collisions: " << collisionStats.total << "\n";
            std::cout << "Mean Collisions: " << collisionStats.mean << "\n";
            std::cout << "Stdev Collisions: " << collisionStats.stdev << "\n";

            std::cout << "Total Number of Lists: " << hashmap_.size() << "\n";
            std::cout << "Number of Lists With Zero Elements: " << numWith0 << "\n";
            std::cout << "Number of Lists with One Element: " << numWith1 << "\n";
            std::cout << "Number of Lists with more than One Element: " << numWithMore << "\n";
            //Histogram?

            std::cout << "Number of elements in the biggest List: " << elements[elements.size()-1] << "\n";
            std::cout << "Number of elements in the smallest List: " << elements[0] << "\n";
        }
        std::cout << std::endl;
    }

    double getLoadFactor() override {
        return (static_cast<float>(getNumPairs()))/(static_cast<float>(hashmap_.size()));
    }

    uint64_t getFullSize() override {
        return hashmap_.size();
    }

    uint64_t getNumPairs() override {
        return numPairs_;
    }

private:
    //default max load factor before doubling size is 0.75
    double MAX_LOAD_FACTOR_ = 0.75;

    struct KeyValuePair{
    public:
        KeyValuePair(Key k, Value v) : key_(k), value_(v) {}
        Key key_;
        Value value_;
    };

    std::vector<std::list<KeyValuePair>> hashmap_;

    uint64_t numPairs_ = 0;

    std::hash<Key> hasher_;

    //individual pairs of types may have custom hash functions
    uint64_t hash(Key k, uint64_t m){
        return hasher_(k) % m;
    }

    void doubleSize(){
        std::vector<std::list<KeyValuePair>> newMap(hashmap_.size() * 2);
        //for every KeyValuePair in the old map, add it to the new map
        for(const auto& bucket : hashmap_){
            for(const auto& kvp : bucket){
                uint64_t index = hash(kvp.key_, hashmap_.size() * 2);
                newMap[index].push_back(kvp);
            }
        }
        hashmap_ = newMap;
    }
    //individual pairs of types may have custom print functions
    void printKeyValuePair(KeyValuePair kvp){
        std::cout << " Invalid types for printing ";
    }

};

template<>
uint64_t ChainedMap<uint64_t, Product*>::hash(uint64_t key, uint64_t mod) {
    uint64_t out[2];
    MurmurHash3_x64_128(&key, sizeof(key), 123, out);
    return out[1] % mod;
}

template<>
uint64_t ChainedMap<uint64_t, uint64_t>::hash(uint64_t key, uint64_t mod) {
    uint64_t out[2];
    MurmurHash3_x64_128(&key, sizeof(key), 123, out);
    return out[1] % mod;
}

template<>
void ChainedMap<std::string, uint64_t>::printKeyValuePair(ChainedMap::KeyValuePair kvp) {
    std::cout << "<" << kvp.key_ << ", " << kvp.value_ << "> ";
}

template<>
void ChainedMap<uint64_t, Product*>::printKeyValuePair(ChainedMap::KeyValuePair kvp) {
    std::cout << "<" << kvp.key_ << ", " << kvp.value_->id << "> ";
}

template<>
void ChainedMap<uint64_t, uint64_t>::printKeyValuePair(ChainedMap::KeyValuePair kvp) {
    std::cout << "<" << kvp.key_ << ", " << kvp.value_ << "> ";
}
