#pragma once
#include "SensorTrajectory.h"


class SensorTrajectoryCADAC : public SensorTrajectory {
    
    public:

    SensorTrajectoryCADAC(std::string loadPath, std::string kmlPath);
    ~SensorTrajectoryCADAC() = default;

    virtual void setSingleCoordsLine();
    virtual void setBITA_Params(); 
    
    float get_vVertical() { return std::stof(utils::SubStringStartTillReaching(this->data_[currentDetectionIndex], ',', 10, 1, currentDetectionIndex, "SensorTrajectory::get_vVertical",true)); } // xxxx does this need synchronization? accesses currentDetectionIndex XXXX the ',', 15, and 1 ARE NOT GENERAL BUT DEPENDENT ON SIMULATION so need to change or atleast comment that this is not the focus of the project and this line can be manipulated by any one who chooses so and i made simplification to myself that arent the interesting or important case.
    
    float getCurrentAlt() { 
        return std::stof(utils::SubStringStartTillReaching(data_[currentDetectionIndex], ',', 6, 1, currentDetectionIndex, "SensorTrajectory::getCurrentAlt",true));
    }

    virtual void plotDataFromRT(SyncObject* syncObject);  // XXXX NAME 

    protected:

    private:

};
