#pragma once

#include <cmath>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

struct Statistic{
    uint64_t total = 0;
    float mean = 0.0;
    float stdev = 0.0;
};

Statistic evaluateStats(const std::vector<uint64_t>* in){
    Statistic statValues;
    if(in->empty()){
        return statValues;
    }
    //total
    statValues.total = std::accumulate(in->begin(), in->end(), 0ULL);
    //mean
    statValues.mean = static_cast<float>(statValues.total)/static_cast<float>(in->size());
    //standard deviation
    //stdev = sqrt ( sum of every ( (element - mean of elements) squared ) / number of elements )
    //which is the same as
    //stdev = sqrt(sigma[ (xi-u)^2 ] / n)
    //where N = size of population, xi = each value from the population, and u = population mean
    float sumSquared = 0;
    for(auto num : *in){
        //pow must be static cast because it returns a double
        sumSquared += static_cast<float>(std::pow(((static_cast<float>(num))-statValues.mean), 2));
    }
    //sqrt must be static cast because it returns a double
    statValues.stdev = static_cast<float>(std::sqrt((static_cast<float>(sumSquared))/(static_cast<float>(in->size()))));

    return statValues;
}
