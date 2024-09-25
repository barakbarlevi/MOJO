#pragma once
#include "Trajectory.h"

class PredictionSupplierTrajectory : public Trajectory {
    public:


    unsigned int currentRowIndex = 0;

    PredictionSupplierTrajectory(std::string loadPath, std::string kmlPath);   // XXXX names.. XXXX ADD consts? when to add const?
    ~PredictionSupplierTrajectory() = default; // COMPLETE IF THERE's memory allocation needed to be deleted. XXXX what's default? why this?

    void appendTrajectoryToKML(int indexJump, int currentSupplierNumber, int CollectorSize, bool isCollector);

    std::thread threadpPlotTrajectoryAtOnce(int indexJump, int currentNumberOfSuppliers, int CollectorSize, bool isCollector) {
        if(!isCollector) {
            utils::kmlInit_href(this->KML_path, this->KML_path, this->color);

        }   // XXXX for simplicity ZMANIT assuming only plotting detections coord-by-coord. it's less meaningful for the prediction suppliers and i dont wanna mess up the code with tons of passed arguments or LEZAMZEM the option of plotting coord by coord to only detections and not predictions.
        
        return std::thread([=]{appendTrajectoryToKML(indexJump, currentNumberOfSuppliers, CollectorSize, isCollector);});
    }

    std::vector<float> altData;
    void setAltData(); 
    std::vector<float> getAltData() {return altData; };    // xxxx what happens if called before readinput?
};