#pragma once

#include "Header.h" // XXXX CHANGE HEADER.H TO SOMETHING MORE OFFICIAL.
#include "Trajectory.h"
#include "PredictionSupplierTrajectory.h"
#include <memory>

class SuppliersCollector {  // XXXX THIS IS SUPPLIERS OR SUPPLIER ? PICK ONE AND GO ALL THROUGHOUT.
    public:

    //nt suppliersCollectorsVectorIndex_;    // MAKE ARRANGEMENTS IN ALL project vars to have rightConventions 
    int currentNumOfSuppliers;
    float timeAfterDetectionCreated_;
    std::string collectorKML_;
    
    //std::vector<std::shared_ptr<Trajectory> > suppliersVector;    // xxxx delete if not used
    std::vector<std::shared_ptr<PredictionSupplierTrajectory> > suppliersVector;
                                

    //SuppliersCollector(int suppliersCollectorsVectorIndex) { this->suppliersCollectorsVectorIndex_= suppliersCollectorsVectorIndex; } // xxxx delete if not needed
    SuppliersCollector(float timeAfterDetectionCreated) {
        this->currentNumOfSuppliers = 0;
        timeAfterDetectionCreated_ = timeAfterDetectionCreated;
    }
    
    int getCurrentNumOfSuppliers() { return this->currentNumOfSuppliers; }



    void plotCollectorAtOnce(int indexJump);  // XXXX if really implementing like this, should it be raw pointer or smart pointer? ownership etc xxxx after implementing plotsupplierallatonce, go over this one and see if all arguments are really needed. looks like this is only for 1 traj, whose kml is already known, and the rest of the args too. xxxx here's a good example of classes are named class MyClass and variables are camelCase. go through all and make sure it's like this throughout xxxx need to have ownership on the object? need to pass smart pointer?

    std::thread threadpPlotCollectorAtOnce(int indexJump) {
        return std::thread([=]{plotCollectorAtOnce(indexJump);});
    }



};