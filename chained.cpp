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
    for(auto keyValuePair : hashmap[index]){
        if(keyValuePair.first == s){
            keyValuePair.second = v;
            return;
        }
    }
    std::pair<std::string, uint32_t> kvp(s, v);
    hashmap[index].push_back(kvp);
    ++numPairs;
    if((static_cast<float>(numPairs))/(static_cast<float>(hashmapSize)) > MAX_LOAD_FACTOR){
        doubleSize();
    }
}

void StringToIntMap::remove(std::string s) {
    uint32_t index = hash(s);
    //for(std::list<std::pair<std::string, uint32_t>>::iterator i = hashmap[index].begin(); i != hashmap[index].end(); advance(i, 1)){
    for(auto i = hashmap[index].begin(); i != hashmap[index].end(); ++i) {
        if(s == i->first){
            hashmap[index].erase(i);
            --numPairs;
            return;
        }
    }
}

void StringToIntMap::doubleSize(){
    hashmapSize *= 2;
    std::vector<std::list<std::pair<std::string, uint32_t>>> newMap(hashmapSize);
    for(auto bucket : hashmap){
        for(auto keyValuePair : bucket){
            uint32_t index = hash(keyValuePair.first);
            newMap[index].push_back(keyValuePair);
        }
    }
    //does this automatically free the old data?
    hashmap = newMap;
}

void StringToIntMap::display(){
    std::vector<uint64_t> elements;
    std::vector<uint64_t> collisions;
    uint64_t numWith0 = 0;
    uint64_t numWith1 = 0;
    uint64_t numWithMore = 0;

    //loop through and print the data structure while storing important information
    for(uint32_t i = 0; i < hashmapSize; i++){
        std::cout << i << ": |" << hashmap[i].size() << "| ";
        elements.push_back(static_cast<uint64_t>(hashmap[i].size()));
        if(hashmap[i].size() == 0) {
            ++numWith0;
            collisions.push_back(0);
        } else {
            collisions.push_back(static_cast<uint64_t>(hashmap[i].size()-1));
            if(hashmap[i].size() == 1){
                ++numWith1;
            }
            else{
                ++numWithMore;
            }
        }
        for(auto x : hashmap[i]){
            std::cout << "<" << x.first << ", " << x.second << "> ";
        }
        std::cout << std::endl;
    }

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


uint32_t IntToProductMap::hash(uint32_t x){
    //this multiplication is random, insecure, and just there to add some randomness to the distribution
    return (hasher(x)*(x+3))%hashmapSize;
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
    for(auto keyValuePair : hashmap[index]){
        if(keyValuePair.first == i){
            keyValuePair.second = p;
            return;
        }
    }
    std::pair<uint32_t, Product*> kvp(i, p);
    hashmap[index].push_back(kvp);
    ++numPairs;
    if((static_cast<float>(numPairs))/(static_cast<float>(hashmapSize)) > MAX_LOAD_FACTOR){
        doubleSize();
    }
}

void IntToProductMap::remove(uint32_t t) {
    uint32_t index = hash(t);
    //for(std::list<std::pair<uint32_t, Product*>>::iterator i = hashmap[index].begin(); i != hashmap[index].end(); advance(i, 1)){
    for(auto i = hashmap[index].begin(); i != hashmap[index].end(); ++i) {
    if(t == i->first){
            hashmap[index].erase(i);
            --numPairs;
            return;
        }
    }
}

void IntToProductMap::doubleSize(){
    hashmapSize *= 2;
    std::vector<std::list<std::pair<uint32_t , Product*>>> newMap(hashmapSize);
    for(auto bucket : hashmap){
        for(auto keyValuePair : bucket){
            uint32_t index = hash(keyValuePair.first);
            newMap[index].push_back(keyValuePair);
        }
    }
    //does this automatically free the old data?
    hashmap = newMap;
}

void IntToProductMap::display(){
    std::vector<uint64_t> elements;
    std::vector<uint64_t> collisions;
    uint64_t numWith0 = 0;
    uint64_t numWith1 = 0;
    uint64_t numWithMore = 0;

    for(uint32_t i = 0; i < hashmapSize; i++){
        std::cout << i << ": |" << hashmap[i].size() << "| ";
        elements.push_back(static_cast<uint64_t>(hashmap[i].size()));
        if(hashmap[i].size() == 0) {
            ++numWith0;
            collisions.push_back(0);
        } else {
            collisions.push_back(static_cast<uint64_t>(hashmap[i].size()-1));
            if(hashmap[i].size() == 1){
                ++numWith1;
            }
            else{
                ++numWithMore;
            }
        }
        for(auto x : hashmap[i]){
            std::cout << "<" << x.first << ", " << x.second->id << "> ";
        }
        std::cout << "\n";
    }

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