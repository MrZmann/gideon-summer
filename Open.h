#pragma once

#include "HashTable.h"
#include "Product.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

template<typename Key, typename Value>
class OpenMap: public HashTable<Key, Value> {
private:
    double MAX_LOAD_FACTOR = 0.7;

    struct KeyValuePair{
        KeyValuePair(Key k, Value v) {
            key = k;
            value = v;
            valid = true;
        }
        KeyValuePair(){
            key = Key{};
            value = Value{};
            valid = false;
        }
        Key key;
        Value value;
        bool valid;
    };

    //this is the operation done on any index that collides
    uint64_t iterate(uint64_t index){
        return index+1;
    }

    std::vector<KeyValuePair>* hashmap;
    std::vector<bool>* significancemap;

    std::hash<Key> hasher;

    uint64_t hash(Key k){
        return hasher(k)%hashmapSize;
    }

    uint64_t hashmapSize = 4;

    uint64_t numPairs = 0;
    uint64_t numSignificant = 0;

    void doubleSize(){
        hashmapSize*= 2;
        auto newMap = new std::vector<KeyValuePair>(hashmapSize);
        auto newSignificance = new std::vector<bool>(hashmapSize);
        for(auto kvp : (*hashmap)){
            if(kvp.valid){
                uint64_t index = hash(kvp.key);
                while((*newSignificance)[index%hashmapSize]){
                    index = iterate(index);
                }
                (*newMap)[index%hashmapSize] = kvp;
                (*newSignificance)[index%hashmapSize] = true;
            }
        }
        numSignificant = numPairs;
        //clear old memory
        auto temp = hashmap;
        auto temp2 = significancemap;
        hashmap = newMap;
        significancemap = newSignificance;
        delete temp;
        delete temp2;
    }

    Value defaultGetReturn(){
        std::cerr << "Invalid types for defaultGetReturn";
        exit(1);
    }

    void printKeyValuePair(KeyValuePair kvp){
        std::cout << " Invalid types for printing ";
    }

public:
    OpenMap() {
        hashmap =  new std::vector<KeyValuePair>(hashmapSize);
        significancemap = new std::vector<bool>(hashmapSize, false);
    }

    OpenMap(double load) {
        hashmap =  new std::vector<KeyValuePair>(hashmapSize);
        significancemap = new std::vector<bool>(hashmapSize, false);
        MAX_LOAD_FACTOR = load;
    }

    ~OpenMap(){
        delete hashmap;
        delete significancemap;
    }

    Value get(Key k) {
        uint64_t index = hash(k);
        while((*significancemap)[index%hashmapSize]){
            if((*hashmap)[index%hashmapSize].valid && k == (*hashmap)[index%hashmapSize].key){
                return (*hashmap)[index%hashmapSize].value;
            }
            index = iterate(index);
        }
        return defaultGetReturn();
    }

    void put(Key k, Value v){
        uint64_t index = hash(k);
        while((*significancemap)[index%hashmapSize]){
            if(!((*hashmap)[index%hashmapSize].valid)){
                break;
            }
            else if(k == (*hashmap)[index%hashmapSize].key){
                (*hashmap)[index%hashmapSize].value = v;
                return;
            }
            index = iterate(index);
        }
        KeyValuePair kvp(k, v);
        (*hashmap)[index%hashmapSize] = kvp;
        (*significancemap)[index%hashmapSize] = true;
        ++numPairs;
        ++numSignificant;
        if((static_cast<float>(numSignificant))/(static_cast<float>(hashmapSize)) > MAX_LOAD_FACTOR){
            doubleSize();
        }
    }

    void remove(Key k) {
        uint64_t index = hash(k);
        while((*significancemap)[index%hashmapSize]){
            if((*hashmap)[index%hashmapSize].valid && k == (*hashmap)[index%hashmapSize].key){
                (*hashmap)[index%hashmapSize] = KeyValuePair();
                --numPairs;
                return;
            }
            index = iterate(index);
        }
    }

    void display(bool print){
        uint64_t numElements = 0;
        for(int i = 0; i < hashmapSize; ++i){
            if((*hashmap)[i].valid) {
                if(print) {
                    std::cout << i << ": {" << (((*significancemap)[i]) ? "T" : "F") << "} |" << hash((*hashmap)[i].key) << "| ";
                    printKeyValuePair((*hashmap)[i]);
                    std::cout << "\n";
                }
                ++numElements;
            }
            else {
                if(print) {
                    std::cout << i << ": {" << (((*significancemap)[i]) ? "T" : "F") << "}\n";
                }
            }
        }
        std::cout << "Total Elements: "<< numElements << "\n";
        std::cout << "Map Size: " << hashmapSize << "\n";
        std::cout << "Load Factor: " << (static_cast<float>(numSignificant))/(static_cast<float>(hashmapSize)) << "\n";
        //# probes
        std::cout << std::endl;
    }
};

template<>
uint64_t OpenMap<std::string, uint64_t>::defaultGetReturn() {
    return -1;
}
template<>
Product* OpenMap<uint64_t, Product*>::defaultGetReturn(){
    return nullptr;
}


template<>
uint64_t OpenMap<std::string, uint64_t>::hash(std::string k){
    return hasher(k)%hashmapSize;
}
template<>
uint64_t OpenMap<uint64_t, Product*>::hash(uint64_t k){
    //this is random, inefficient math that exists to somewhat randomize placement
    return (static_cast<uint64_t>(floor((sqrt(static_cast<double>(hasher(k)))*static_cast<double>(k+3)))))%hashmapSize;
}


template<>
void OpenMap<std::string, uint64_t>::printKeyValuePair(OpenMap::KeyValuePair kvp) {
    std::cout << "<" << kvp.key << ", " << kvp.value << "> ";
}

template<>
void OpenMap<uint64_t, Product*>::printKeyValuePair(OpenMap::KeyValuePair kvp) {
    std::cout << "<" << kvp.key << ", " << kvp.value->id << "> ";
}