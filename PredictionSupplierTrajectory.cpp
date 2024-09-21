#include "PredictionSupplierTrajectory.h"

PredictionSupplierTrajectory::PredictionSupplierTrajectory(std::string loadPath, std::string kmlPath) : Trajectory(loadPath, kmlPath) {}; // XXXX names...

void PredictionSupplierTrajectory::setAltData() {
    for (size_t i = this->FirstLineOfNumericData_; i < this->data.size(); i++) {
        std::string alt = utils::SubStringStartTillReaching(data[i], ',', 6, 1, i, "PredictionSupplierTrajectory::setAltData()"); // XXXX check on func arguments to match cadac / general(?)
        altData.push_back(std::stof(alt));
    }   
}