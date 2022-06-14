#include "Chained.h"
#include "Open.h"
#include "StdMap.h"
#include "StdUnorderedMap.h"

#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <stdlib.h>

enum MAP_TYPE {STDMAP = 0, CHAINEDMAP = 1, OPENMAP = 2, STDUNORDEREDMAP = 3};

void read_args(int, char**);

//command line options
//defaults are what they are set to here
MAP_TYPE mapType_ = STDMAP;
uint64_t verbose_ = 0;
uint64_t mapSize_ = 100000;
double maxLoadFactor_ = 0.7;

//given a map type, size, and max load factor, this function will make a new graph, fill it with garbage data, and then call get on every piece of data.
//It will record the time to fill, time to read, and some statistics about the graph's final position and system resource consumption.
void testBuildTime(uint64_t mapSize, MAP_TYPE mapType, double maxLoadFactor);


int main(int32_t argc, char** argv){
    //command line options:
    //-m: mapType: std (default), stdunordered, chained, or open
    //-s: mapSize: any uint64_t (default 100,000)
    //-l: maxLoadFactor: any double (default 0.7) (0..1 for OPENMAP) (discarded for STDMAP and STDUNORDEREDMAP)
    //-v: verbose: default 0, 1 to enable, 2 to enable and print graph
    read_args(argc, argv);
    testBuildTime(mapSize_, mapType_, maxLoadFactor_);
}


void testBuildTime(uint64_t mapSize, MAP_TYPE mapType, double maxLoadFactor){
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



    //populate map
    for(uint64_t i = 0; i < mapSize; ++i){
        //this overflows around i = 4.29 billion, but that shouldn't cause any problems as the value will just wrap back to 0
        map->put(i, (i+1)*(i+3));
    }

    //test getting 10x the number of itmes in the map
    srand (time(NULL));
    for(uint64_t i = 0; i < (10*mapSize); i++){
    	uint64_t j = rand()%mapSize;
		if ((j+1)*(j+3) != map->get(j)) {
			std::cerr << "Map implementation broken\n";
			exit(1);
		}
    }
 
    if(verbose_ >= 2){
        map->display();
    }

    if(verbose_ >= 1){
        map->stats();
    }

    delete map;
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
