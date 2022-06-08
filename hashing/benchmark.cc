#include "Chained.h"
#include "Open.h"
#include "StdMap.h"
#include "StdUnorderedMap.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <cstring>
#include <sys/time.h>
#include <sys/resource.h>

enum MAP_TYPE {STDMAP = 0, CHAINEDMAP = 1, OPENMAP = 2, STDUNORDEREDMAP = 3};

void read_args(int, char**);

//command line options
//defaults are what they are set to here
MAP_TYPE mapType_ = STDMAP;
uint64_t verbose_ = 0;
uint64_t mapSize_ = 100000;
double maxLoadFactor_ = 0.7;

struct graphStat{
    //the -1 values will be filled in later, and if they aren't, it signals that the data point was inaccessible for that hashmap type
    graphStat(MAP_TYPE t, double l, uint64_t n) : mapType(t), maxLoadFactor(l), numberOfItems(n), msToFill(-1), msToRead(-1), finalLoadFactor(-1.0) {}

    MAP_TYPE mapType;
    double maxLoadFactor;
    uint64_t numberOfItems;
    uint64_t msToFill;
    uint64_t msToRead;
    double finalLoadFactor;

    std::string getType();
    std::string toCSV();
};

//given a map type, size, and max load factor, this function will make a new graph, fill it with garbage data, and then call get on every piece of data.
//It will record the time to fill, time to read, and some statistics about the graph's final position and system resource consumption.
graphStat testBuildTime(uint64_t mapSize, MAP_TYPE mapType, double maxLoadFactor);

std::string rUsageToCSV(rusage stat);

int main(int32_t argc, char** argv){
    //command line options:
    //-m: mapType: std (default), stdunordered, chained, or open
    //-s: mapSize: any uint64_t (default 100,000)
    //-l: maxLoadFactor: any double (default 0.7) (0..1 for OPENMAP) (discarded for STDMAP and STDUNORDEREDMAP)
    //-v: verbose: default 0, 1 to enable, 2 to enable and print graph
    read_args(argc, argv);
    graphStat stat = testBuildTime(mapSize_, mapType_, maxLoadFactor_);
    rusage machineStats;
    if(getrusage(RUSAGE_SELF, &machineStats) != 0){
        std::cerr << "getrusage failed";
        exit(1);
    }

    std::cout << stat.toCSV() << rUsageToCSV(machineStats) << std::endl;
}

std::string graphStat::getType() {
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


std::string rUsageToCSV(rusage stat){
    std::string csv;
    csv += std::to_string((stat.ru_stime.tv_sec * 1000ULL) + (stat.ru_stime.tv_usec / 1000ULL));
    csv+= ",";
    csv += std::to_string(stat.ru_maxrss);
    csv+= ",";
    return csv;
}

//Graph Type,Number of Items,Max Load Factor,Final Load Factor,ms to Fill,ms to Read,
std::string graphStat::toCSV() {
    std::string csv;
    csv += getType();
    csv += ",";
    csv += std::to_string(numberOfItems);
    csv += ",";
    csv += std::to_string(maxLoadFactor);
    csv += ",";
    csv += std::to_string(finalLoadFactor);
    csv += ",";
    csv += std::to_string(msToFill);
    csv += ",";
    csv += std::to_string(msToRead);
    csv += ",";
    return csv;
}

graphStat testBuildTime(uint64_t mapSize, MAP_TYPE mapType, double maxLoadFactor){
    //init map
    HashTable<uint64_t, uint64_t>* map;
    switch(mapType){
        case CHAINEDMAP:
            map = new ChainedMap<uint64_t, uint64_t>(maxLoadFactor);
            break;
        case OPENMAP:
            map = new OpenMap<uint64_t, uint64_t>(maxLoadFactor);
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
    graphStat graphStatistics(mapType, maxLoadFactor, mapSize);

    if(verbose_ >= 1) {
        std::cout << "Initializing " << graphStatistics.getType() << " with a max load factor of " << maxLoadFactor << "\n";
    }

    //start clock for filling map
    auto time1 = std::chrono::high_resolution_clock::now();


    //populate map first 1/2
    for(uint64_t i = 0; i < mapSize/2; ++i){
        //this overflows around i = 4.29 billion, but that shouldn't cause any problems as the value will just wrap back to 0
        map->put(i, (i+1)*(i+3));
    }
    //stop clock for filling map and record the time it took in milliseconds
    auto time2 = std::chrono::high_resolution_clock::now();
    auto durMs = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);
    if(verbose_ >= 1) {
        std::cout << "Time to fill " << graphStatistics.getType() << " with a max load factor of " << maxLoadFactor << ": "
                  << durMs.count() << " milliseconds" << std::endl;
    }
    graphStatistics.msToFill = durMs.count();

    //start clock for getting every item in the map
    time1 = std::chrono::high_resolution_clock::now();
    //test getting every item in the map

    srand (time(NULL));
    for(uint64_t i = 0; i < (5*mapSize); i++){
    	uint64_t j = rand()%(mapSize/2);
		if ((j+1)*(j+3) != map->get(j)) {
			std::cerr << "Map implementation broken\n";
			exit(1);
		}
    }
    //stop clock for getting every item in the map and record the time it took in milliseconds
    time2 = std::chrono::high_resolution_clock::now();
    durMs = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);
    if(verbose_ >= 1) {
        std::cout << "Time to get every item in " << graphStatistics.getType() << ": " << durMs.count()
                  << " milliseconds\n" << std::endl;
    }
    graphStatistics.msToRead = durMs.count();
    graphStatistics.finalLoadFactor = map->getLoadFactor();
	
	//populate map second 1/2
	time1 = std::chrono::high_resolution_clock::now();
    for(uint64_t i = mapSize/2; i < mapSize; ++i){
        //this overflows around i = 4.29 billion, but that shouldn't cause any problems as the value will just wrap back to 0
        map->put(i, (i+1)*(i+3));
    }
    //stop clock for filling map and record the time it took in milliseconds
    time2 = std::chrono::high_resolution_clock::now();
    durMs = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);
    if(verbose_ >= 1) {
        std::cout << "Time to fill " << graphStatistics.getType() << " with a max load factor of " << maxLoadFactor << ": "
                  << durMs.count() << " milliseconds" << std::endl;
    }
    graphStatistics.msToFill += durMs.count();
    
    
    //start clock for getting every item in the map
    time1 = std::chrono::high_resolution_clock::now();
    //test getting every item in the map
    srand (time(NULL));
    for(uint64_t i = 0; i < (10*mapSize); i++){
    	uint64_t j = rand()%mapSize;
		if ((j+1)*(j+3) != map->get(j)) {
			std::cerr << "Map implementation broken\n";
			exit(1);
		}
    }
    //stop clock for getting every item in the map and record the time it took in milliseconds
    time2 = std::chrono::high_resolution_clock::now();
    durMs = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);
    if(verbose_ >= 1) {
        std::cout << "Time to get every item in " << graphStatistics.getType() << ": " << durMs.count()
                  << " milliseconds\n" << std::endl;
    }
    graphStatistics.msToRead += durMs.count();
    graphStatistics.finalLoadFactor = map->getLoadFactor();
	

    if(verbose_ >= 2){
        map->display();
    }

    if(verbose_ >= 1){
        map->stats();
    }

    delete map;
    return graphStatistics;
}

void read_args(int32_t argc, char** argv){
    int32_t opt;
    while((opt = getopt(argc, argv, ":m:s:l:v:h")) != -1)
    {
        switch(opt)
        {
            case 'v':
                //verbose
                //try-catch?
                verbose_ = atoi(optarg);
                break;
            case 'h':
                //help
                std::cout << "Usage: test [arguments]\n";
                std::cout << "\t-m:\tmapType: std (default), stdunordered, chained, or open\n";
                std::cout << "\t-s:\tmapSize: any uint64_t (default 100,000)\n";
                std::cout << "\t-l:\tmaxLoadFactor: any double (default 0.7) (0..1 for OPENMAP) (discarded for STDMAP and STDUNORDEREDMAP)\n";
                std::cout << "\t-v:\tverbose: default 0, 1 to enable, 2 to enable and print graph\n";
                exit(0);
            case 'm':
                //map type
                if(strcmp("chained", optarg) == 0){
                    mapType_ = CHAINEDMAP;
                } else if(strcmp("std", optarg) == 0){
                    mapType_ = STDMAP;
                } else if(strcmp("open", optarg) == 0){
                    mapType_ = OPENMAP;
                } else if (strcmp("stdunordered", optarg) == 0){
                    mapType_ = STDUNORDEREDMAP;
                } else {
                    std::cerr << "Unexpected map type!\n";
                    exit(1);
                }
                break;
            case 's':
                //map size
                //try-catch?
                mapSize_ = atol(optarg);
                break;
            case 'l':
                //max load factor
                //try-catch?
                maxLoadFactor_ = atof(optarg);
                break;
            default:
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    if(verbose_ > 0){
        //start by printing the basic map settings
        std::cout << "Map type: ";
        if(mapType_ == STDMAP)
            std::cout << "STDMAP\n";
        else if(mapType_ == CHAINEDMAP)
            std::cout << "CHAINEDMAP\n";
        else if(mapType_ == OPENMAP)
            std::cout << "OPENMAP\n";
        else if(mapType_ == STDUNORDEREDMAP)
            std::cout << "STDUNORDEREDMAP\n";
        std::cout << "Map size: " << mapSize_ << "\n";
        if(mapType_==STDMAP || mapType_==STDUNORDEREDMAP){
            std::cout << "Max load factor discarded - ignore verbose load factor messaging\n";
        }
        else{
            std::cout << "Max load factor: " << maxLoadFactor_ << "\n";
        }
        std::cout << std::endl;
    }
}
