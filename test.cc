#include "product.h"

#ifdef USE_STD_MAP
    #include "stdmap.h"
#endif

#ifdef USE_CHAINED_MAP
    #include "chained.h"
#endif


#include <iostream>
#include <vector>

int main(){
    StringToIntMap name_to_id;
    IntToProductMap inventory;

    // Populate inventory
    uint32_t NUM_PRODUCTS = 4;
    std::string products[4] = {"Twix", "Snickers", "Butterfingers", "Reese's"};
    int id = 0;
    for(uint32_t i = 0; i < NUM_PRODUCTS; i++){
        std::string name = products[i];
        name_to_id.put(name, i);
        Product* prod = new Product(i, i, i);
        inventory.put(i, prod);
    }

    for(uint32_t i = 0; i < NUM_PRODUCTS; i++){
        if(i != name_to_id.get(products[i])){
            std::cerr << "Map implementation broken\n";
            exit(1);
        }
        auto prod = inventory.get(i);
        std::cout << products[i] << "\n";
        std::cout << "\tQuantity: " << prod->quantity << "\n";
        std::cout << "\tPrice: " << prod->price << "\n";
        std::cout << std::endl;
    }

    inventory.remove(3);
    if(inventory.get(3) != NULL){
        std::cerr << "Remove failed\n";
        exit(1);

    }

}
