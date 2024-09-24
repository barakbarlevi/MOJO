#include "SuppliersCollector.h"
#include "SensorTrajectoryCADAC.h"

class DecisionMaker {
    public:
    
    //SuppliersCollectorsVector* collectorsVector_;
    std::vector<std::shared_ptr<SuppliersCollector>>* collectorsVector_;
    
    //SensorTrajectory* sensorTrajectory_; // xxxx check whether this is upcast/downcast
    SensorTrajectoryCADAC* sensorTrajectory_;
    
    float estimation_;
    
    // DecisionMaker(SuppliersCollectorsVector* suppliersCollectorsVector, SensorTrajectoryCADAC* sensorTrajectory) {
    //     collectorsVector_ = suppliersCollectorsVector;
    //     sensorTrajectory_ = sensorTrajectory;
    //     estimation_ = 0;
    // }

    DecisionMaker(std::vector<std::shared_ptr<SuppliersCollector>>* suppliersCollectorsVector, SensorTrajectoryCADAC* sensorTrajectory) {
        collectorsVector_ = suppliersCollectorsVector;
        sensorTrajectory_ = sensorTrajectory;
        estimation_ = 0;
    }

    void calculate(SyncDataArrivalAndPredicting * syncSingleton);

    std::thread threadCalculate(SyncDataArrivalAndPredicting * syncSingleton) {
        return std::thread([=] {calculate(syncSingleton);});
    }

};