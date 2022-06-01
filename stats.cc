#include "stats.h"

Statistic* evaluateStats(const std::vector<uint64_t>* in){
    Statistic* statValues = new Statistic;
    if(in->empty()){
        return statValues;
    }
    //total
    statValues->total = std::accumulate(in->begin(), in->end(), 0UL);
    //mean
    statValues->mean = static_cast<float>(statValues->total)/in->size();
    //standard deviation
    float sumSquared = 0;
    for(auto x : *in){
        sumSquared += pow(((static_cast<float>(x))-statValues->mean), 2);
    }
    statValues->stdev = sqrt((static_cast<float>(sumSquared))/(static_cast<float>(in->size())));

    return statValues;
}