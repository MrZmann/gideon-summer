#pragma once

#include <iostream>

class Product{
    public:
        uint32_t id;
        uint32_t quantity;
        uint32_t price;

        Product(uint32_t id, uint32_t quantity, uint32_t price) : 
            id(id),
            quantity(quantity), 
            price(price) { }

};

