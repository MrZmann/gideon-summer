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

Statistic* evaluateStats(const std::vector<uint64_t>* in);