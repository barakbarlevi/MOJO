#pragma once
#include "Trajectory.h"
#include "SyncDataArrivalAndPredicting.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



class SensorTrajectory : public Trajectory {
    protected:
    
    float vVertical = 0;    // XXXX name sucks. XXXX explain on assumed coordinate system. up is down etc

    public:

    SensorTrajectory(std::string loadPath, std::string kmlPath);   // ADD CONSTS? WHEN TO ADD CONSTS? XXXX
    ~SensorTrajectory() = default; // COMPLETE IF THERE's memory allocation needed to be deleted. XXXX what's default? why this?
    virtual void setBITA_Params() = 0;  // XXXX names...
    BITA_params getBITA_Params() { return this->_BITA_Params; } // XXXX names...
    
    //float get_vVertical() { return std::stof(utils::SubStringStartTillReaching(this->data[currentDetectionIndex], ',', 10, 1)); } // XXXX the ',', 15, and 1 ARE NOT GENERAL BUT DEPENDENT ON SIMULATION so need to change or atleast comment that this is not the focus of the project and this line can be manipulated by any one who chooses so and i made simplification to myself that arent the interesting or important case.
    float get_vVertical() { return std::stof(utils::SubStringStartTillReaching(this->data[currentDetectionIndex], ',', 10, 1, currentDetectionIndex, "SensorTrajectory::get_vVertical")); } // xxxx does this need synchronization? accesses currentDetectionIndex XXXX the ',', 15, and 1 ARE NOT GENERAL BUT DEPENDENT ON SIMULATION so need to change or atleast comment that this is not the focus of the project and this line can be manipulated by any one who chooses so and i made simplification to myself that arent the interesting or important case.
    
    float getCurrentAlt() { 
        return std::stof(utils::SubStringStartTillReaching(data[currentDetectionIndex], ',', 6, 1, currentDetectionIndex, "SensorTrajectory::getCurrentAlt"));
    } // xxxx does this need synchronization? accesses currentDetectionIndex


    std::thread threadReceiveDataFromRT(SyncDataArrivalAndPredicting* syncSingleton) {

        utils::kmlInit_href(this->KML_path, this->KML_path, this->color);
        return std::thread([=]{plotDataFromRT(syncSingleton);});

    }   // XXXX as of now i'm taking into consideration only 1 detection and predictofrs for it. that's why it has a defined style, written in Source.cpp. mention that if anybody wants to expand to multiple detections inspection simultaneously, then can pretty easily modify it. but not right now.

    virtual void plotDataFromRT(SyncDataArrivalAndPredicting* syncSingleton) = 0; // xxxx check that this is right..

/*
    // xxxx why the hell is this method not in SensorTrajectoy ?
    std::thread threadplotTrajectoryCoordByCoord(int indexJump, int delayms) {
        utils::kmlInit_href(this->KML_path, this->KML_path, this->color);
        return std::thread([=]{appendTrajectoryToKML(indexJump, delayms);});
    }   // XXXX as of now i'm taking into consideration only 1 detection and predictofrs for it. that's why it has a defined style, written in Source.cpp. mention that if anybody wants to expand to multiple detections inspection simultaneously, then can pretty easily modify it. but not right now.
*/

};