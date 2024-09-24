#include "SuppliersCollector.h"

void SuppliersCollector::plotCollectorAtOnce(int indexJump) {

    //std::string kmlName = "SupplierCollector" + std::to_string(this->suppliersCollectorsVectorIndex_);    // xxxx rename this variable. name is specifically the "name" field inside the kml. something between path and name
    //utils::kmlInit_href(kmlName, kmlName, suppliersVector.size()); // XXXX what's the meaning of this? make better. also: to_string.. xxxx why the hell pass twice both name and kml...
    utils::kmlInit_href(this->collectorKML_, this->collectorKML_, suppliersVector.size()); // XXXX what's the meaning of this? make better. also: to_string.. xxxx why the hell pass twice both name and kml...
    
    for(size_t i = 0; i < suppliersVector.size(); i++) {
        
        std::shared_ptr<Trajectory> supplierTrajectory = this->suppliersVector.at(i);
        //utils::kmlInsertEntireTrajectory(sdf, kmlName, indexJump, i, this->currentNumOfSuppliers, styleScale, 0, 0);    //  XXXX names etc    
        supplierTrajectory->appendTrajectoryToKML(indexJump, i, this->suppliersVector.size(), 1);
        
    }

    utils::kmlInsertOneNetworkLink("Secondary_Controller.kml",this->collectorKML_); // xxxx names!  
        
}
