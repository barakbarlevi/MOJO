#pragma once
#include "SuppliersCollector.h"




#include "BITA_params.h"

// xxxx minimize includes
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <functional>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <mutex>
#include <condition_variable>
#include <math.h>




class SuppliersCollectorsVector {
    public:

    int currentNumOfSuppliersCollectors_;
    std::vector<std::shared_ptr<SuppliersCollector>> suppliersCollectorsVector_;

    SuppliersCollectorsVector() {currentNumOfSuppliersCollectors_ = 0;};
    int getCurrentNumOfSuppliersCollectors() { return currentNumOfSuppliersCollectors_;}
};