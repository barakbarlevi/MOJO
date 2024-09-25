#include "PredictionSupplierTrajectory.h"

PredictionSupplierTrajectory::PredictionSupplierTrajectory(std::string loadPath, std::string kmlPath) : Trajectory(loadPath, kmlPath) {}; // XXXX names...

void PredictionSupplierTrajectory::setAltData() {
    for (size_t i = this->FirstLineOfNumericData_; i < this->data_.size(); i++) {
        std::string alt = utils::SubStringStartTillReaching(data_[i], ',', 6, 1, i, "PredictionSupplierTrajectory::setAltData()",false); // XXXX check on func arguments to match cadac / general(?)
        altData.push_back(std::stof(alt));
    }   
}

void PredictionSupplierTrajectory::appendTrajectoryToKML(int indexJump, int currentSupplierNumber, int CollectorSize, bool isCollector) {
        
        (void)CollectorSize;    // xxxx had to pass two ints that have different meaning! can't overload like that cuz both int so had to put a third arg! english. consult someone about this maybe
        
        if(!isCollector) utils::kmlInsertOneNetworkLink("Secondary_Controller.kml",this->kmlPath_); // xxxx names!    
        // xxxx if it is a collector, the collector will do it for itself ENGLISH
        
        this->currentRowIndex = this->FirstLineOfNumericData_; // XXXX raw pointers ? move to smart ? or is it ok when a function argument? i think that its possible to pass smart pointers as function args..? https://stackoverflow.com/questions/65035189/whether-to-pass-shared-pointer-or-raw-pointer-to-a-function . do i take ownership here?
      
        //std::cout << "Inserting Coord-By-Coord CADAC Trajectory to KML: " << this->kmlPath_ << " at index jumps of: " << indexJump << std::endl; // XXXX English. check that this cout is needed. fix what needs to be fixed. maybe remove that cout maybe not
        for (unsigned int i = this->currentRowIndex; i < this->data_.size() / indexJump; i++) {
            // currentRowIndex = i * indexJump; // XXXX makes sense to put in comment, add comment about it.
            this->setSingleCoordsLine();
            if(isCollector) {
                utils::kmlAppendOneCoord(this->kmlPath_, this->SingleCoordsLine_, std::to_string(currentSupplierNumber + 1)); // XXXX HERE this->SingleCoordsLine_ and line above just SingleCoordsLine_ ?xxxx fix this method to the one i printed on one paper that only does appending
            } else {
                utils::kmlAppendOneCoord(this->kmlPath_, this->SingleCoordsLine_, "0");
            }
            
                   
            this->currentRowIndex += indexJump; // XXXX fix name of indexJump (?)
        }
        
    }    