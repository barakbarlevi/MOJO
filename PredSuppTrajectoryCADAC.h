#pragma once

#include "PredictionSupplierTrajectory.h"

class PredSuppTrajectoryCADAC : public PredictionSupplierTrajectory {
    
    public :

    PredSuppTrajectoryCADAC(std::string loadPath, std::string kmlPath);   // XXXX

    //virtual int PlotTrajectoryCoordByCoord(int indexJump);    // XXXX first letter capital or not ?
    
    //virtual int kmlInsertEntireTrajectory(std::string KML, int indexJump, int currentNumOfSuppliers, int CollectorSize, float StyleScale);    // XXXX first letter capital or not? names
    virtual void setSingleCoordsLine();  // XXXX name etc
    //virtual int PlotTrajectoryAtOnce(std::string KML, int indexJump, int currentNumbebrOfSuppliers, int CollectorSize, float StyleScale);    //  XXXX    A FIGURE EXPLAINING THE LAYOUT: MAIN TRAJECTORY SENSOR, SUPPLIER, COLLOCTOR, ETC. can be done with screenshop from ge.    XXXX delete if not needed here
};