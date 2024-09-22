#include "SuppliersCollectorsVector.h"

//#include "SensorTrajectory.h"
#include "SensorTrajectoryCADAC.h"

//extern int globalcount; // xxxx

class DecisionMaker {
    public:
    //std::shared_ptr<PredSuppTrajectoryCADAC> trajectoryCADAC;   // XXXX CHANGE NAME
    SuppliersCollectorsVector* collectorsVector_;
    
    //SensorTrajectory* sensorTrajectory_; // xxxx check whether this is upcast/downcast
    SensorTrajectoryCADAC* sensorTrajectory_;
    
    float estimation_;
    
    //DecisionMaker(SuppliersCollectorsVector* suppliersCollectorsVector, SensorTrajectory* sensorTrajectory) {
    DecisionMaker(SuppliersCollectorsVector* suppliersCollectorsVector, SensorTrajectoryCADAC* sensorTrajectory) {
        collectorsVector_ = suppliersCollectorsVector;
        sensorTrajectory_ = sensorTrajectory;
        estimation_ = 0;
    }

    void calculate(SyncDataArrivalAndPredicting * syncSingleton);

    std::thread threadCalculate(SyncDataArrivalAndPredicting * syncSingleton) {
        return std::thread([=] {calculate(syncSingleton);});
    }

};