#include "PredictionSupplierTrajectory.h"

PredictionSupplierTrajectory::PredictionSupplierTrajectory(std::string loadPath, std::string kmlPath) : Trajectory(loadPath, kmlPath) {}; // XXXX names...

void PredictionSupplierTrajectory::setAltData() {
    for (size_t i = this->FirstLineOfNumericData_; i < this->data_.size(); i++) {
        std::string alt = utils::SubStringStartTillReaching(data_[i], ',', 6, 1, i, "PredictionSupplierTrajectory::setAltData()",false); // XXXX check on func arguments to match cadac / general(?)
        altData.push_back(std::stof(alt));
    }   
}

void PredictionSupplierTrajectory::appendTrajectoryToKML(int effective_dtPlot, int currentSupplierNumber, int CollectorSize, bool isCollector) {
    
    (void)CollectorSize;
    if(!isCollector) utils::kmlInsertOneNetworkLink("Secondary_Controller.kml",this->kmlPath_);
    this->currentRowIndex = this->FirstLineOfNumericData_;
    
    for (unsigned int i = this->currentRowIndex; i < this->data_.size() / effective_dtPlot; i++) {
        // currentRowIndex = i * effective_dtPlot; // Optional
        this->setSingleCoordsLine();
        if(isCollector) {
            utils::kmlAppendOneCoord(this->kmlPath_, this->SingleCoordsLine_, std::to_string(currentSupplierNumber + 1));
        } else {
            utils::kmlAppendOneCoord(this->kmlPath_, this->SingleCoordsLine_, "0");
        }
        this->currentRowIndex += effective_dtPlot;
    }
}    