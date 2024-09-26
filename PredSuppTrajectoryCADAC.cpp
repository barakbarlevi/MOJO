#include "PredSuppTrajectoryCADAC.h"

PredSuppTrajectoryCADAC::PredSuppTrajectoryCADAC(std::string loadPath, std::string kmlPath) : PredictionSupplierTrajectory(loadPath, kmlPath) { 
    this->FirstLineOfNumericData_ = 0; 
    this->currentRowIndex = this->FirstLineOfNumericData_;
    
};

// XXXX CHANGE ENGLISH Not investing time in implementing it because not planning to plot a supplier coord by coord. Just wrote it so project will compile.
//int PredSuppTrajectoryCADAC::PlotTrajectoryCoordByCoord(int effective_dtPlot) { return 0; }




void PredSuppTrajectoryCADAC::setSingleCoordsLine()
{
    

    std::string lon = utils::SubStringStartTillReaching(this->data_[this->currentRowIndex], ',', 4, 1, currentRowIndex, "PredSuppTrajectoryCADAC::setSingleCoordsLine 1",false); // XXXX check on func arguments to match cadac / general(?)  XXXX here i put this-> and below not. do both work? why? which to choose?
    std::string lat = utils::SubStringStartTillReaching(data_[currentRowIndex], ',', 5, 1, currentRowIndex, "PredSuppTrajectoryCADAC::setSingleCoordsLine 2",false); // XXXX check on func arguments to match cadac / general(?)
    std::string alt = utils::SubStringStartTillReaching(data_[currentRowIndex], ',', 6, 1, currentRowIndex, "PredSuppTrajectoryCADAC::setSingleCoordsLine 3",false); // XXXX check on func arguments to match cadac / general(?)


    this->SingleCoordsLine_ = lon + "," + lat + "," + alt;   // XXXX THERE MUST be a much more efficient way... ask chatgpt or think... at the end, in enhancements

} // XXXX names etc

//int PredSuppTrajectoryCADAC::PlotTrajectoryAtOnce(std::string KML, int effective_dtPlot, int currentNumbebrOfSuppliers, int CollectorSize, float StyleScale) {};