#include "chained.h"

uint32_t StringToIntMap::hash(std::string x){
    return hasher(x)%hashmapSize;
}

uint32_t StringToIntMap::get(std::string s) {
    uint32_t index = hash(s);
    for(auto x : hashmap[index]){
        if(s == x.first){
            return x.second;
        }
    }
    return -1;
}

void StringToIntMap::put(std::string s, uint32_t v) {
    uint32_t index = hash(s);
    std::pair<std::string, uint32_t> keyValuePair(s, v);
    hashmap[index].push_back(keyValuePair);
}

void StringToIntMap::remove(std::string s) {
    uint32_t index = hash(s);
    for(std::list<std::pair<std::string, uint32_t>>::iterator i = hashmap[index].begin(); i != hashmap[index].end(); advance(i, 1)){
        if(s == i->first){
            hashmap[index].erase(i);
        }
    }
}

void StringToIntMap::display(){
    for(int i = 0; i < hashmapSize; i++){
        std::cout << i << ": ";
        for(auto x : hashmap[i]){
            std::cout << "<" << x.first << ", " << x.second << "> ";
        }
        std::cout << "\n";
    }
}


uint32_t IntToProductMap::hash(uint32_t x){
    return hasher(x)%hashmapSize;
}

Product* IntToProductMap::get(uint32_t s) {
    uint32_t index = hash(s);
    for(auto x : hashmap[index]){
        if(s == x.first){
            return x.second;
        }
    }
    return nullptr;
}

void IntToProductMap::put(uint32_t i, Product* p) {
    uint32_t index = hash(i);
    std::pair<uint32_t, Product*> keyValuePair(i, p);
    hashmap[index].push_back(keyValuePair);
}

void IntToProductMap::remove(uint32_t t) {
    uint32_t index = hash(t);
    for(std::list<std::pair<uint32_t, Product*>>::iterator i = hashmap[index].begin(); i != hashmap[index].end(); advance(i, 1)){
        if(t == i->first){
            hashmap[index].erase(i);
        }
    }
}

void IntToProductMap::display(){
    for(int i = 0; i < hashmapSize; i++){
        std::cout << i << ": ";
        for(auto x : hashmap[i]){
            std::cout << "<" << x.first << ", " << x.second->id << "> ";
        }
        std::cout << "\n";
    }
}