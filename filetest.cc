#include "Product.h"
#include "Chained.h"

#include <iostream>
#include <vector>
#include <fstream>

int main(){

    auto name_to_id = new ChainedMap<std::string, uint32_t>();
    auto inventory = new ChainedMap<uint32_t, Product*>();

    // Populate inventory
    std::ifstream read("../in.txt");
    uint32_t NUM_PRODUCTS;
    std::vector<std::string> products;
    std::string temp;
    if(read.is_open()){
        read >> NUM_PRODUCTS;
        for(int i = 0; i < NUM_PRODUCTS; i++){
            read >> temp;
            products.push_back(temp);
        }
    }
    read.close();

    //fill hashmap
    for(uint32_t i = 0; i < NUM_PRODUCTS; i++){
        std::string name = products[i];
        name_to_id->put(name, i);
        Product* prod = new Product(i, i, i);
        inventory->put(i, prod);
    }

    /*
    //check against original
    for(uint32_t i = 0; i < NUM_PRODUCTS; i++){
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
    */

    name_to_id->display(true);
    std::cout << std::endl;
    inventory->display(false);
    std::cout << std::endl;

    inventory->remove(3);
    inventory->remove(127);
    inventory->remove(1111);
    inventory->remove(1234);
    inventory->remove(2222);

    if(inventory->get(3) != NULL || inventory->get(127) != NULL || inventory->get(1111) != NULL || inventory->get(1234) != NULL || inventory->get(2222) != NULL){
        std::cerr << "Remove failed\n";
        exit(1);

    }


    name_to_id->display(false);
    std::cout << std::endl;
    inventory->display(false);
    std::cout << std::endl;

}
