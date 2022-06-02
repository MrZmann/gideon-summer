#include "Chained.h"
#include "HashTable.h"
#include "Open.h"
#include "Product.h"
#include "StdMap.h"

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>


int main() {
    OpenMap<std::string, uint64_t> *name_to_id = new OpenMap<std::string, uint64_t>();
    OpenMap<uint64_t, Product *> *inventory = new OpenMap<uint64_t, Product *>();

    // Populate inventory
    uint64_t NUM_PRODUCTS = 4;
    std::string products[4] = {"Twix", "Snickers", "Butterfingers", "Reese's"};
    for (uint64_t i = 0; i < NUM_PRODUCTS; i++) {
        std::string name = products[i];
        name_to_id->put(name, i);
        Product *prod = new Product(i, i, i);
        inventory->put(i, prod);
    }

    for (uint64_t i = 0; i < NUM_PRODUCTS; i++) {
        if (i != name_to_id->get(products[i])) {
            std::cerr << "Map implementation broken\n";
            exit(1);
        }
        auto prod = inventory->get(i);
        std::cout << products[i] << "\n";
        std::cout << "\tQuantity: " << prod->quantity << "\n";
        std::cout << "\tPrice: " << prod->price << "\n";
        std::cout << std::endl;
    }
    name_to_id->display(true);
    std::cout << std::endl;
    //inventory->display(true);
    //std::cout << std::endl;

    name_to_id->remove("Twix");
    name_to_id->remove("Reese's");

    if (name_to_id->get("Twix") != -1) {
        std::cerr << "Remove failed\n";
        exit(1);

    }
    name_to_id->display(true);
    std::cout << std::endl;
    //inventory->display(true);
    //std::cout << std::endl;

    name_to_id->put("Reese's", 3);
    name_to_id->put("Twix", 0);

    for (uint64_t i = 0; i < NUM_PRODUCTS; i++) {
        if (i != name_to_id->get(products[i])) {
            std::cerr << "Map implementation broken\n";
            exit(1);
        }
        auto prod = inventory->get(i);
        std::cout << products[i] << "\n";
        std::cout << "\tQuantity: " << prod->quantity << "\n";
        std::cout << "\tPrice: " << prod->price << "\n";
        std::cout << std::endl;
    }


    name_to_id->display(true);
    std::cout << std::endl;
    //inventory->display(true);
   // std::cout << std::endl;
}