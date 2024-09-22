#pragma once

#include "SensorTrajectory.h"


class SensorTrajectoryCADAC : public SensorTrajectory {
    
    public:

    SensorTrajectoryCADAC(std::string loadPath, std::string kmlPath); // XXXX NAMES. XXXX ADD consts? when to add const?
    //SensorTrajectoryCADAC(std::string kmlPath); // xxxx for the real time case. don't have a load path. XXXX NAMES. XXXX ADD consts? when to add const?

    virtual void setBITA_Params();  // XXXX NAME 
    
    //virtual int PlotTrajectoryCoordByCoord(int indexJump);    // XXXX NAme

    //virtual int kmlInsertEntireTrajectory(std::string KML, int indexJump, int currentNumOfSuppliers, int CollectorSize, float StyleScale);    // XXXX ADD FULLY DETAILED DESCRIPTION LIKE FISHMAN DID IN COBRA ON ALL METHODS ALL variables all functions. Change names here and wherever need be.
    virtual void setSingleCoordsLine();  // XXXX names etc
    // XXXX ADD A TEMPLATE .h and maybe .pp files all in comments for other users to use with their general simulation.
    //virtual int PlotTrajectoryAtOnce(std::string KML, int indexJump, int currentNumbebrOfSuppliers, int CollectorSize, float StyleScale);    //  XXXX    A FIGURE EXPLAINING THE LAYOUT: MAIN TRAJECTORY SENSOR, SUPPLIER, COLLOCTOR, ETC. can be done with screenshop from ge.    XXXX delete if not needed here

    virtual void plotDataFromRT(SyncDataArrivalAndPredicting* syncSingleton);  // XXXX NAME 
};
