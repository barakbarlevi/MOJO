#pragma once
#include "Trajectory.h"

class PredictionSupplierTrajectory : public Trajectory {
    public:

    PredictionSupplierTrajectory(std::string loadPath, std::string kmlPath);   // XXXX names.. XXXX ADD consts? when to add const?
    ~PredictionSupplierTrajectory() = default; // COMPLETE IF THERE's memory allocation needed to be deleted. XXXX what's default? why this?

    std::vector<float> altData;
    void setAltData(); 
    std::vector<float> getAltData() {return altData; };    // xxxx what happens if called before readinput?
};