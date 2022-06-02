#include "Chained.h"
#include "HashTable.h"
#include "Open.h"
#include "Product.h"
#include "StdMap.h"

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>

void read_args(int, char**);
enum MAP_TYPE {CHAINED_MAP, OPEN_MAP, STD_MAP };

// Command line options
MAP_TYPE mapType = STD_MAP;
int verbose = 0;



int main(int argc, char** argv){
    read_args(argc, argv);

    HashTable<std::string, uint64_t>* name_to_id;
    HashTable<uint64_t, Product*>* inventory;
    
    if(mapType == CHAINED_MAP){
        name_to_id = new ChainedMap<std::string, uint64_t>();
        inventory = new ChainedMap<uint64_t, Product*>();

    } else if(mapType == OPEN_MAP){
        name_to_id = new OpenMap<std::string, uint64_t>();
        inventory = new OpenMap<uint64_t, Product*>();

    } else if(mapType == STD_MAP){
        name_to_id = new StdMap<std::string, uint64_t>();
        inventory = new StdMap<uint64_t, Product*>();
    } else {
        std::cerr << "Unexpected map type\n";
        exit(1);
    }

    // Populate inventory
    uint64_t NUM_PRODUCTS = 4;
    std::string products[4] = {"Twix", "Snickers", "Butterfingers", "Reese's"};
    for(uint64_t i = 0; i < NUM_PRODUCTS; i++){
        std::string name = products[i];
        name_to_id->put(name, i);
        Product* prod = new Product(i, i, i);
        inventory->put(i, prod);
    }

    for(uint64_t i = 0; i < NUM_PRODUCTS; i++){
        if(i != name_to_id->get(products[i])){
            std::cerr << "Map implementation broken\n";
            exit(1);
        }
        auto prod = inventory->get(i);
        std::cout << products[i] << "\n";
        std::cout << "\tQuantity: " << prod->quantity << "\n";
        std::cout << "\tPrice: " << prod->price << "\n";
        std::cout << std::endl;
    }

    inventory->remove(3);
    if(inventory->get(3) != NULL){
        std::cerr << "Remove failed\n";
        exit(1);

    }

}
void read_args(int argc, char** argv){
    int opt;
    while((opt = getopt(argc, argv, ":m:hv")) != -1)
    {
        switch(opt)
        {
            case 'v':
                verbose = 1;
                break;
            case 'h':
                std::cout << "Usage: test [arguments]\n";
                std::cout << "\t-m:\tmapType: std (default), chained, or open\n";
                std::cout << "\t-v:\tverbose\n";
                exit(0);
            case 'm':
                if(strcmp("chained", optarg) == 0){
                    mapType = CHAINED_MAP;
                } else if(strcmp("std", optarg) == 0){
                    mapType = STD_MAP;
                } else if(strcmp("open", optarg) == 0){
                    mapType = OPEN_MAP;
                } else {
                    std::cerr << "Unexpected map type!\n";
                    exit(1);
                }
                break;
            default:  
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    if(verbose){
        std::cout << "Map type: ";
        if(mapType == STD_MAP)      
            std::cout << "STD_MAP\n";
        else if(mapType == CHAINED_MAP)  
            std::cout << "CHAINED_MAP\n";
        else if(mapType == OPEN_MAP) 
            std::cout << "OPEN_MAP\n";
    }
}

