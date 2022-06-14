#pragma once

#include <iostream>

class Product{
    public:
        uint64_t id;
        uint64_t quantity;
        uint64_t price;

        Product(uint64_t id, uint64_t quantity, uint64_t price) :
            id(id),
            quantity(quantity), 
            price(price) { }

};

