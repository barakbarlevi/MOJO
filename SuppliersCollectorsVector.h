#pragma once
#include "SuppliersCollector.h"
#include "Header.h"

class SuppliersCollectorsVector {
    public:

    int currentNumOfSuppliersCollectors_;
    std::vector<std::shared_ptr<SuppliersCollector>> suppliersCollectorsVector_;

    SuppliersCollectorsVector() {currentNumOfSuppliersCollectors_ = 0;};
    int getCurrentNumOfSuppliersCollectors() { return currentNumOfSuppliersCollectors_;}
};