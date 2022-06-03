#include "Product.h"
#include "Chained.h"
#include "Open.h"
#include "StdMap.h"
#include "StdUnorderedMap.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>

enum MAP_TYPE {STDMAP = 0, CHAINEDMAP = 1, OPENMAP = 2, STDUNORDEREDMAP = 3};

void read_args(int, char**);

//command line options
MAP_TYPE mapType = STDMAP;
uint64_t verbose = 0;
uint64_t mapSize = 100000;
double load = 0.7;

struct productsArr{
    std::string* products;
    uint64_t size;
};

struct graphStat{
    graphStat(MAP_TYPE t, double l, uint64_t n) {
        mapType = t;
        loadFactor = l;
        numberOfItems = n;
        msToFill = -1;
        msToRead = -1;
    }
    MAP_TYPE mapType;
    double loadFactor;
    uint64_t numberOfItems;
    uint64_t msToFill;
    uint64_t msToRead;

    std::string getType(){
        switch (mapType) {
            case STDMAP:
                return "STD Map";
            case CHAINEDMAP:
                return "Chained Map";
            case OPENMAP:
                return "Open Map";
            case STDUNORDEREDMAP:
                return "STD Unordered Map";
        }
        return "";
    }
};

bool compareMsFill(graphStat a, graphStat b) {
    return a.msToFill < b.msToFill;
}

bool compareMsRead(graphStat a, graphStat b) {
    return a.msToRead < b.msToRead;
}

graphStat testBuildTime(const uint64_t mapSize, MAP_TYPE mapType, double loadFactor){
    auto time1 = std::chrono::high_resolution_clock::now();
    auto time2 = std::chrono::high_resolution_clock::now();
    HashTable<uint64_t, uint64_t>* map;
    switch(mapType){
        case CHAINEDMAP:
            map = new ChainedMap<uint64_t, uint64_t>(loadFactor);
            break;
        case OPENMAP:
            map = new OpenMap<uint64_t, uint64_t>(loadFactor);
            break;
        case STDMAP:
            map = new StdMap<uint64_t, uint64_t>;
            break;
        case STDUNORDEREDMAP:
            map = new StdUnorderedMap<uint64_t, uint64_t>;
            break;
        default:
            std::cerr << "Unexpected map type\n";
            exit(1);
    }
    graphStat graphStatistics(mapType, loadFactor, mapSize);

    if(verbose >= 1) {
        std::cout << "Initializing " << graphStatistics.getType() << " with a load factor of " << loadFactor << "\n";
    }
    //populate map
    for(uint64_t i = 0; i < mapSize; ++i){
        map->put(i, (i+1)*(i+3));
    }

    time2 = std::chrono::high_resolution_clock::now();
    auto durMs = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);
    if(verbose >= 1) {
        std::cout << "Time to fill " << graphStatistics.getType() << " with a load factor of " << loadFactor << ": "
                  << durMs.count() << " milliseconds" << std::endl;
    }
    graphStatistics.msToFill = durMs.count();

    //testing map getting
    time1 = std::chrono::high_resolution_clock::now();
    for(uint64_t i = 0; i < mapSize; i++) {
        if ((i+1)*(i+3) != map->get(i)) {
            std::cerr << "Map implementation broken\n";
            exit(1);
        }
    }
    time2 = std::chrono::high_resolution_clock::now();
    durMs = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);
    if(verbose >= 1) {
        std::cout << "Time to get every item in " << graphStatistics.getType() << ": " << durMs.count()
                  << " milliseconds\n" << std::endl;
    }
    graphStatistics.msToRead = durMs.count();

    delete map;
    return graphStatistics;
}

int main(int32_t argc, char** argv){
    read_args(argc, argv);
    //also compare against unordered map

    graphStat stat = testBuildTime(mapSize, mapType, load);
    std::cout << stat.getType() << "," << std::to_string(stat.numberOfItems) << "," << std::to_string(stat.loadFactor) << "," << std::to_string(stat.msToFill) << "," << std::to_string(stat.msToRead) << ",\n";
}

void read_args(int32_t argc, char** argv){
    int32_t opt;
    //why is there no colon between h and v?
    while((opt = getopt(argc, argv, ":m:s:l:hv")) != -1)
    {
        switch(opt)
        {
            case 'v':
                verbose = 1;
                break;
            case 'h':
                std::cout << "Usage: test [arguments]\n";
                std::cout << "\t-m:\tmapType: std (default), stdunordered, chained, or open\n";
                std::cout << "\t-s:\tmapSize: any uint64_t (default 100,000)\n";
                std::cout << "\t-l:\tLoadFactor: any double (default 0.7) (0..1 for OPENMAP) (discarded for STDMAP and STDUNORDEREDMAP)\n";
                std::cout << "\t-v:\tverbose: 1 to enable (default 0)\n";
                exit(0);
            case 'm':
                if(strcmp("chained", optarg) == 0){
                    mapType = CHAINEDMAP;
                } else if(strcmp("std", optarg) == 0){
                    mapType = STDMAP;
                } else if(strcmp("open", optarg) == 0){
                    mapType = OPENMAP;
                } else if (strcmp("stdunordered", optarg) == 0){
                    mapType = STDUNORDEREDMAP;
                } else {
                    std::cerr << "Unexpected map type!\n";
                    exit(1);
                }
                break;
            case 's':
                //try-catch?
                mapSize = atol(optarg);
                break;
            case 'l':
                //try-catch?
                load = atof(optarg);
                break;
            default:
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    if(verbose){
        std::cout << "Map type: ";
        if(mapType == STDMAP)
            std::cout << "STDMAP\n";
        else if(mapType == CHAINEDMAP)
            std::cout << "CHAINEDMAP\n";
        else if(mapType == OPENMAP)
            std::cout << "OPENMAP\n";
        else if(mapType == STDUNORDEREDMAP)
            std::cout << "STDUNORDEREDMAP\n";
    }
}
