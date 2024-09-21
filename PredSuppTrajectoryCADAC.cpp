// XXXX EVERY LINE OF CODE SAYING CADAC SHOULD BE SWITCHED TO SOMETHING GENERAL. or maybe explain that it's general until at some level u have to be percise on simulation. but as less as possible.

#include "PredSuppTrajectoryCADAC.h"
#include "utils.h"

extern std::mutex KML_editing_mutex;    // XXXX consistency on capital and non capital lettes. XXXX explain on extern.

PredSuppTrajectoryCADAC::PredSuppTrajectoryCADAC(std::string loadPath, std::string kmlPath) : PredictionSupplierTrajectory(loadPath, kmlPath) { 
    this->FirstLineOfNumericData_ = 0; 
    this->currentRowIndex = this->FirstLineOfNumericData_;
    
};

// XXXX CHANGE ENGLISH Not investing time in implementing it because not planning to plot a supplier coord by coord. Just wrote it so project will compile.


//int PredSuppTrajectoryCADAC::PlotTrajectoryCoordByCoord(int indexJump) { return 0; }


/*
int PredSuppTrajectoryCADAC::kmlInsertEntireTrajectory(std::string KML, int indexJump, int currentNumOfSuppliers, int CollectorSize, float StyleScale) { // XXXX names... edit..
    std::cout << "Inserting entire CADAC Trajectory to KML: " << KML << " at index jumps of: " << indexJump << std::endl;   // XXXX English. check that this cout is needed. fix what needs to be fixed.
    std::string line;
    utils::rgb RGB;
    utils::hsv HSV;
    std::stringstream ss;

    HSV.h = ((((double)currentNumOfSuppliers) / (double)CollectorSize)) * 360.0;    // XXXX THERES ABSOLUTELY no way that so many casts are needed. MINIMIZE.
    HSV.s = 0.9;
    HSV.v = 1;
    RGB = utils::hsv2rgb(HSV);

    int decimalR = static_cast<int>(RGB.r * 255);
    int decimalG = static_cast<int>(RGB.g * 255);
    int decimalB = static_cast<int>(RGB.b * 255);
    int decimalA = (1 * 255);

    ss << std::hex << std::setfill('0') << std::setw(2) << decimalA << std::hex <<std::setfill('0') << std::setw(2) << decimalG << std::hex << std::setfill('0') << std::setw(2) << decimalB << std::hex << std::setfill('0') << std::setw(2) << decimalR << std::endl;
    std::string color = ss.str();   // XXXX ss.str() ? FROm what library this and c_str, to_string .substr() etc are from ?

    std::unique_lock<std::mutex> EditKML_ul(KML_editing_mutex); // XXXX name

    //this->kmlInsertOneStyle(KML, std::to_string(currentNumOfSuppliers), color.substr(0, color.size() - 1), std::to_string(StyleScale)); // XXXX if uncessecary, delete
    utils::kmlInsertOneStyle(KML, std::to_string(currentNumOfSuppliers), color.substr(0, color.size() - 1), std::to_string(StyleScale));  // XXXX names etc.

    currentRowIndex = 4;    // XXXX WHY THE HECK 4? this code needs to be seen by others. parameterize.
    for(unsigned int i = 4; i < this->data.size() / indexJump; i++) {   // XXXX unsigned int ? i++ so why had there ++i ? strange ++i
        // currentRowIndex = i * indexJump; // XXXX makes sense to put in comment, add comment about it.
        line = data[currentRowIndex];
        SingleCoordsLine = line.substr(78,6) + "," + line.substr(66,6) + "," + line.substr(18,2) + line.substr(21,3);   // XXXX change ofc
        
        //this->kmlAppendOneCoord(KML, std::to_string(currentNumOfSuppliers), "Supplier");    // XXXX delete if not needed anymore
        utils::kmlAppendOneCoord(KML, this->SingleCoordsLine, std::to_string(currentNumOfSuppliers), "Supplier"); // XXXX HERE this->SingleCoordsLine and line above just SingleCoordsLine ?

        currentRowIndex += indexJump;   // XXXX fix name of indexJump (?)
    }

    EditKML_ul.unlock();    // XXXX challenge the fact that mutexes are really where they should be. all throughout the code.

    return 0;
}
*/

void PredSuppTrajectoryCADAC::setSingleCoordsLine()
{
    /*
    std::string lon = utils::SubStringStartTillReaching(this->data[this->currentRowIndex], ',', 4, 1); // XXXX check on func arguments to match cadac / general(?)  XXXX here i put this-> and below not. do both work? why? which to choose?
    std::string lat = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 5, 1); // XXXX check on func arguments to match cadac / general(?)
    std::string alt = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 6, 1); // XXXX check on func arguments to match cadac / general(?)
    */

    std::string lon = utils::SubStringStartTillReaching(this->data[this->currentRowIndex], ',', 4, 1, currentRowIndex, "PredSuppTrajectoryCADAC::setSingleCoordsLine 1"); // XXXX check on func arguments to match cadac / general(?)  XXXX here i put this-> and below not. do both work? why? which to choose?
    std::string lat = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 5, 1, currentRowIndex, "PredSuppTrajectoryCADAC::setSingleCoordsLine 2"); // XXXX check on func arguments to match cadac / general(?)
    std::string alt = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 6, 1, currentRowIndex, "PredSuppTrajectoryCADAC::setSingleCoordsLine 3"); // XXXX check on func arguments to match cadac / general(?)


    this->SingleCoordsLine = lon + "," + lat + "," + alt;   // XXXX THERE MUST be a much more efficient way... ask chatgpt or think... at the end, in enhancements

} // XXXX names etc

//int PredSuppTrajectoryCADAC::PlotTrajectoryAtOnce(std::string KML, int indexJump, int currentNumbebrOfSuppliers, int CollectorSize, float StyleScale) {};