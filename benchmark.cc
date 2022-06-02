#include "Product.h"
#include "Chained.h"
#include "Open.h"
#include "StdMap.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

struct productsArr{
    std::string* products;
    uint64_t size;
};

enum MAP_TYPE {STDGRAPH = 0, CHAINEDGRAPH = 1, OPENGRAPH = 2};

struct graphStat{
    graphStat(MAP_TYPE t, double l, uint64_t n) {
        graphType = t;
        loadFactor = l;
        numberOfItems = n;
        msToFill = -1;
        msToRead = -1;
    }
    MAP_TYPE graphType;
    double loadFactor;
    uint64_t numberOfItems;
    uint64_t msToFill;
    uint64_t msToRead;
};

void printStat(graphStat stat){
    std::string name;
    switch (stat.graphType){
        case STDGRAPH:
            name = "STD Map";
            break;
        case CHAINEDGRAPH:
            name = "Chained Map";
            break;
        case OPENGRAPH:
            name = "Open Map";
            break;
    }
    std::cout << name << " with " << std::to_string(stat.numberOfItems) << " items | LF: " << std::to_string(stat.loadFactor) << " | ms to Fill: " << std::to_string(stat.msToFill) << " | ms to Read: " << std::to_string(stat.msToRead) << "|\n";
}

bool compareMsFill(graphStat a, graphStat b) {
    return a.msToFill < b.msToFill;
}

bool compareMsRead(graphStat a, graphStat b) {
    return a.msToRead < b.msToRead;
}

graphStat testBuildTime(const std::string* products, const uint64_t productsSize, const MAP_TYPE graphType, const double loadFactor){
    HashTable<std::string, uint64_t>* name_to_id;
    HashTable<uint64_t, Product*>* inventory;
    auto time1 = std::chrono::high_resolution_clock::now();
    auto time2 = std::chrono::high_resolution_clock::now();
    auto durMs = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);
    std::string graphTypeName;
    graphStat graphStatistics(graphType, loadFactor, productsSize);
    switch (graphType){
        case STDGRAPH:
            graphTypeName = "STD Map";
            std::cout << "Initializing STD Map (load factor is thrown away)\n";
            name_to_id = new StdMap<std::string, uint64_t>();
            inventory = new StdMap<uint64_t, Product*>();

            for(uint64_t i = 0; i < productsSize; ++i){
                name_to_id->put(products[i], i);
                auto prod = new Product(i, i, i);
                inventory->put(i, prod);
            }

            time2 = std::chrono::high_resolution_clock::now();
            durMs = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);
            std::cout << "Time to fill STD Map: " << durMs.count() << " milliseconds" << std::endl;
            graphStatistics.msToFill = durMs.count();
            break;
        case CHAINEDGRAPH:
            graphTypeName = "Chained Map";
            std::cout << "Initializing Chain Map with a load factor of " << loadFactor << "\n";
            name_to_id = new ChainedMap<std::string, uint64_t>(loadFactor);
            inventory = new ChainedMap<uint64_t, Product*>(loadFactor);

            for(uint64_t i = 0; i < productsSize; ++i){
                name_to_id->put(products[i], i);
                auto prod = new Product(i, i, i);
                inventory->put(i, prod);
            }

            time2 = std::chrono::high_resolution_clock::now();
            durMs = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);
            std::cout << "Time to fill Chained Map with a load factor of "<< loadFactor << ": " << durMs.count() << " milliseconds" << std::endl;
            graphStatistics.msToFill = durMs.count();
            break;
        case OPENGRAPH:
            graphTypeName = "Open Map";
            if(loadFactor <= 0 || loadFactor >= 1){
                std::cerr << "Invalid load factor for OpenGraph";
                exit(1);
            }
            std::cout << "Initializing Open Map with a load factor of " << loadFactor << "\n";
            name_to_id = new OpenMap<std::string, uint64_t>(loadFactor);
            inventory = new OpenMap<uint64_t, Product*>(loadFactor);

            for(uint64_t i = 0; i < productsSize; ++i){
                name_to_id->put(products[i], i);
                auto prod = new Product(i, i, i);
                inventory->put(i, prod);
            }

            time2 = std::chrono::high_resolution_clock::now();
            durMs = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);
            std::cout << "Time for Open Map with a load factor of "<< loadFactor << ": " << durMs.count() << " milliseconds" << std::endl;
            graphStatistics.msToFill = durMs.count();
            break;
        default:
            std::cerr << "Invalid graph type\n";
            exit(1);
    }
    //testing map getting
    time1 = std::chrono::high_resolution_clock::now();
    for(uint64_t i = 0; i < productsSize; i++) {
        if (i != name_to_id->get(products[i])) {
            std::cerr << "Map implementation broken\n";
            exit(1);
        }
    }
    time2 = std::chrono::high_resolution_clock::now();
    durMs = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);
    std::cout << "Time to get every item in " << graphTypeName << ": " << durMs.count() << " milliseconds\n" << std::endl;
    graphStatistics.msToRead = durMs.count();
    //are the destructors of name_to_id and inventory automatically called because they existed only in this function?
    delete name_to_id;
    delete inventory;
    return graphStatistics;
}

productsArr init(std::string filePath){
    auto start = std::chrono::high_resolution_clock::now();
    // Populate inventory
    std::ifstream read(filePath);
    productsArr productsArray;
    if(read.is_open()){
        read >> productsArray.size;
        productsArray.products = new std::string[productsArray.size];
        for(uint64_t i = 0; i < productsArray.size; ++i){
            read >> productsArray.products[i];
        }
    }
    else{
        std::cerr << "Unable to read from file\n";
        exit(1);
    }
    read.close();

    auto finishedReading = std::chrono::high_resolution_clock::now();
    auto durMs = std::chrono::duration_cast<std::chrono::milliseconds>(finishedReading-start);
    std::cout << "Time to read from file: " << durMs.count() << " milliseconds\n" << std::endl;
    return productsArray;
}

int main(){
    productsArr prods = init("../in.txt");

    std::vector<graphStat> stats;
    stats.push_back(testBuildTime(prods.products, prods.size, STDGRAPH, -1.0));
    stats.push_back(testBuildTime(prods.products, prods.size, CHAINEDGRAPH, 0.75));
    stats.push_back(testBuildTime(prods.products, prods.size, CHAINEDGRAPH, 2));
    stats.push_back(testBuildTime(prods.products, prods.size, CHAINEDGRAPH, 5));
    stats.push_back(testBuildTime(prods.products, prods.size, CHAINEDGRAPH, 10));
    stats.push_back(testBuildTime(prods.products, prods.size, CHAINEDGRAPH, 15));
    stats.push_back(testBuildTime(prods.products, prods.size, OPENGRAPH, 0.3));
    stats.push_back(testBuildTime(prods.products, prods.size, OPENGRAPH, 0.5));
    stats.push_back(testBuildTime(prods.products, prods.size, OPENGRAPH, 0.7));

    std::string name;
    std::cout << "Default Order:\n";
    for(auto stat : stats){
        printStat(stat);
    }
    std::cout << std::endl;

    std::cout << "Sorted by Time to Fill:\n";
    std::sort(stats.begin(), stats.end(), compareMsFill);
    for(auto stat : stats){
        printStat(stat);
    }
    std::cout << std::endl;

    std::cout << "Sorted by Time to Read:\n";
    std::sort(stats.begin(), stats.end(), compareMsRead);
    for(auto stat : stats){
        printStat(stat);
    }
    std::cout << std::endl;


    delete prods.products;
}
