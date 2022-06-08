#pragma once

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
    statValues.total = std::accumulate(in->begin(), in->end(), 0UL);
    //mean
    statValues.mean = static_cast<float>(statValues.total)/in->size();
    //standard deviation
    float sumSquared = 0;
    for(auto num : *in){
        sumSquared += pow(((static_cast<float>(num))-statValues.mean), 2);
    }
    statValues.stdev = sqrt((static_cast<float>(sumSquared))/(static_cast<float>(in->size())));

    return statValues;
}
