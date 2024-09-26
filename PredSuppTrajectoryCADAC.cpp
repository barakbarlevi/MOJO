#include "PredSuppTrajectoryCADAC.h"

PredSuppTrajectoryCADAC::PredSuppTrajectoryCADAC(std::string loadPath, std::string kmlPath) : PredictionSupplierTrajectory(loadPath, kmlPath) { 
    this->FirstLineOfNumericData_ = 0; 
    this->currentRowIndex = this->FirstLineOfNumericData_;
    
};

void PredSuppTrajectoryCADAC::setSingleCoordsLine()
{
    std::string lon = utils::SubStringStartTillReaching(this->data_[this->currentRowIndex], ',', 4, 1, currentRowIndex, "PredSuppTrajectoryCADAC::setSingleCoordsLine 1",false); // XXXX check on func arguments to match cadac / general(?)  XXXX here i put this-> and below not. do both work? why? which to choose?
    std::string lat = utils::SubStringStartTillReaching(data_[currentRowIndex], ',', 5, 1, currentRowIndex, "PredSuppTrajectoryCADAC::setSingleCoordsLine 2",false); // XXXX check on func arguments to match cadac / general(?)
    std::string alt = utils::SubStringStartTillReaching(data_[currentRowIndex], ',', 6, 1, currentRowIndex, "PredSuppTrajectoryCADAC::setSingleCoordsLine 3",false); // XXXX check on func arguments to match cadac / general(?)

    this->SingleCoordsLine_ = lon + "," + lat + "," + alt;
}