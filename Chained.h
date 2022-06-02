#pragma once

#include "HashTable.h"
#include "Product.h"
#include "stats.h"
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>

template<typename Key, typename Value>
class ChainedMap : public HashTable<Key, Value> {
private:
    double MAX_LOAD_FACTOR = 0.75;

    struct KeyValuePair{
        KeyValuePair(Key k, Value v) {
            key = k;
            value = v;
        }
        Key key;
        Value value;
    };

    std::vector<std::list<KeyValuePair>>* hashmap;

    std::hash<Key> hasher;

    uint64_t hash(Key k){
        return hasher(k)%hashmapSize;
    }

    uint64_t hashmapSize = 4;

    uint64_t numPairs = 0;

    void doubleSize(){
        hashmapSize *= 2;
        auto newMap = new std::vector<std::list<KeyValuePair>>(hashmapSize);
        for(auto bucket : (*hashmap)){
            for(auto kvp : bucket){
                uint64_t index = hash(kvp.key);
                (*newMap)[index].push_back(kvp);
            }
        }
        auto temp = hashmap;
        hashmap = newMap;
        delete temp;
    }

    Value defaultGetReturn(){
        std::cerr << "Invalid types for defaultGetReturn";
        exit(1);
    }

    void printKeyValuePair(KeyValuePair kvp){
        std::cout << " Invalid types for printing ";
    }

public:
    ChainedMap() {
        hashmap = new std::vector<std::list<KeyValuePair>>(hashmapSize);
    }

    ChainedMap(double load) {
        hashmap = new std::vector<std::list<KeyValuePair>>(hashmapSize);
        MAX_LOAD_FACTOR = load;
    }

    ~ChainedMap() {
        delete hashmap;
    }

    Value get(Key k){
        uint64_t index = hash(k);
        for(auto kvp : (*hashmap)[index]){
            if(k == kvp.key){
                return kvp.value;
            }
        }
        return defaultGetReturn();
    }

    void put(Key k, Value v) {
        uint64_t index = hash(k);
        for(auto kvp : (*hashmap)[index]){
            if(kvp.key == k){
                kvp.value = v;
                return;
            }
        }
        KeyValuePair kvp(k, v);
        (*hashmap)[index].push_back(kvp);
        ++numPairs;
        if((static_cast<float>(numPairs))/(static_cast<float>(hashmapSize)) > MAX_LOAD_FACTOR){
            doubleSize();
        }
    }

    void remove(Key k) {
        uint64_t index = hash(k);
        for(auto i = (*hashmap)[index].begin(); i != (*hashmap)[index].end(); ++i) {
            if(k == i->key){
                (*hashmap)[index].erase(i);
                --numPairs;
                return;
            }
        }
    }

    void display(bool print){
        std::vector<uint64_t> elements;
        std::vector<uint64_t> collisions;
        uint64_t numWith0 = 0;
        uint64_t numWith1 = 0;
        uint64_t numWithMore = 0;

        //loop through and print the data structure while storing important information
        for(uint64_t i = 0; i < hashmapSize; i++){
            if(print) {
                std::cout << i << ": |" << (*hashmap)[i].size() << "| ";
            }
            elements.push_back(static_cast<uint64_t>((*hashmap)[i].size()));
            if((*hashmap)[i].empty()) {
                ++numWith0;
                collisions.push_back(0);
            } else {
                collisions.push_back(static_cast<uint64_t>((*hashmap)[i].size()-1));
                if((*hashmap)[i].size() == 1){
                    ++numWith1;
                }
                else{
                    ++numWithMore;
                }
            }
            for(auto kvp : (*hashmap)[i]){
                if(print) {
                    printKeyValuePair(kvp);
                }
            }
            if(print) {
                std::cout << "\n";
            }
        }
        std::cout << std::endl;

        //stats output
        if(elements.empty()){
            std::cout << "No Elements\n";
        } else {
            std::sort(elements.begin(), elements.end());
            Statistic* elementStats = evaluateStats(&elements);
            std::cout << "Total Elements: " << elementStats->total << "\n";
            std::cout << "Mean Elements: " << elementStats->mean << "\n";
            std::cout << "Stdev Elements: " << elementStats->stdev << "\n";

            std::sort(collisions.begin(), collisions.end());
            Statistic* collisionStats = evaluateStats(&collisions);
            std::cout << "Total Collisions: " << collisionStats->total << "\n";
            std::cout << "Mean Collisions: " << collisionStats->mean << "\n";
            std::cout << "Stdev Collisions: " << collisionStats->stdev << "\n";

            std::cout << "Total Number of Lists: " << hashmapSize << "\n";
            std::cout << "Number of Lists With Zero Elements: " << numWith0 << "\n";
            std::cout << "Number of Lists with One Element: " << numWith1 << "\n";
            std::cout << "Number of Lists with more than One Element: " << numWithMore << "\n";
            //I could make a display of all element sizes. Box plot? Scatterplot?

            std::cout << "Number of elements in the biggest List: " << elements[elements.size()-1] << "\n";
            std::cout << "Number of elements in the smallest List: " << elements[0] << "\n";
        }
    }
};

template<>
uint64_t ChainedMap<std::string, uint64_t>::defaultGetReturn() {
    return -1;
}
template<>
Product* ChainedMap<uint64_t, Product*>::defaultGetReturn(){
    return nullptr;
}


template<>
uint64_t ChainedMap<std::string, uint64_t>::hash(std::string k){
    return hasher(k)%hashmapSize;
}
template<>
uint64_t ChainedMap<uint64_t, Product*>::hash(uint64_t k){
    //this is random, inefficient math that exists to somewhat randomize placement
    return (static_cast<uint64_t>(floor((sqrt(static_cast<double>(hasher(k)))*(k+3)))))%hashmapSize;
}


template<>
void ChainedMap<std::string, uint64_t>::printKeyValuePair(ChainedMap::KeyValuePair kvp) {
    std::cout << "<" << kvp.key << ", " << kvp.value << "> ";
}

template<>
void ChainedMap<uint64_t, Product*>::printKeyValuePair(ChainedMap::KeyValuePair kvp) {
    std::cout << "<" << kvp.key << ", " << kvp.value->id << "> ";
}