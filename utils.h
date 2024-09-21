#pragma once
#include "Header.h"
//#include "Trajectory.h"
#include <memory>

extern std::mutex KML_editing_mutex;    // XXXX name. explanation on extern.

namespace utils {
    
    std::string SubStringStartTillReaching(const std::string &str, char c, int NumOfEncounterToExclude, int functionality, int currentRowIndex,  std::string caller); // xxxx after solving the problem, delete the currentrowindex and caller arguments
    void replaceLastWord(std::string &line, const std::string& old_word, const std::string& new_word);
    std::string replaceLastWord(const std::string& line, const std::string& newWord);
    //predictionSuppliers hashIt(std::string const & inString); // xxxx delete if not needed

    // xxxx is it possible, will it compile, if moveing this to Header.h?
    typedef struct {
        double h;   // Angle in degrees
        double s;   // Fraction between 0 and 1
        double v;   // Fraction between 0 and 1
    } hsv;

    // xxxx is it possible, will it compile, if moveing this to Header.h?
    typedef struct {
        double r;   // Fraction between 0 and 1
        double g;   // Fraction between 0 and 1
        double b;   // Fraction between 0 and 1
    } rgb;

    hsv rgb2hsv(rgb in);
    rgb hsv2rgb(hsv in);
    
    int kmlInitPrimaryController();
    int kmlInitSecondaryController();
    //int kmlInitTrajectory(std::string kml_path, std::string name);    // XXXX change name to sometime like kmlInitTrajectory XXXX CALL IT CONTROLLING KML OR SOMETHING IN THE SIGNON XXXX Keep this one, to init the two contollers.
    //int kmlInitSuppliersCollector(std::string kml_path, std::string name, int CollectorSize);    // XXXX change name to sometime like kmlInitTrajectory XXXX CALL IT CONTROLLING KML OR SOMETHING IN THE SIGNON XXXX Keep this one, to init the two contollers.
    
    // xxxx should replace kmlInitTrajectory and kmlInitSuppliersCollector
    int kmlInit_href(std::string kml_path, std::string name, std::string color);   // xxxx use this when plotting the sensor. would have used a collector with 1, but then the colors would be the same. explain this in good neglish
    int kmlInit_href(std::string kml_path, std::string name, int CollectorSize);

    int kmlInsertOneNetworkLink(std::string kml_path, std::string href); // XXXX change all relevant function names to KML_functionality
    //int kmlInsertOneStyle(std::string KML, std::string styleID, std::string color, std::string scale); // XXXX names, XXXX more? XXXX MAKE IT WITH ::in ::out ::binary and append by known fixed number of bytes. not destrot and re-write which is stupid. XXXX and then change name to insertOneStyle...
    //int kmlInsertStyles(std::string KML, std::string styleID, std::string color, std::string scale); // XXXX names, xxxx mention: assuming number of suppliers doesn't increase with time (will work if it decreases or stays the same). this assumtion doesn't necessarily have to hold in the general case. doing easy life for me right now. xxxx improve english. the advantage is that when dealing with a supplierscollector that already has for example n-1 suppliers in its kml, i don't have to push down all that data in order to add a style. putting them there in advance.
    //int kmlInsertEntireTrajectory(std::shared_ptr<Trajectory> trajectory, std::string KML, int indexJump, int currentNumbebrOfSuppliers, int CollectorSize, float StyleScale, bool flagDelay, int delayms);    //  XXXX    A FIGURE EXPLAINING THE LAYOUT: MAIN TRAJECTORY SENSOR, SUPPLIER, COLLOCTOR, ETC. can be done with screenshop from ge.
    int kmlAppendOneCoord(std::string KML, std::string SingleCoordsLine, std::string styleID); // XXXX Kinda looks like bad practive to have these arguments. see if can modify and write better.   
    // xxxx when ready, delete this ! and the implementation as well !
    //int AppendOneCoordInKML(std::string KML, std::string SingleCoordsLine, std::string styleID, std::string SensorOrSupplier); // XXXX Kinda looks like bad practive to have these arguments. see if can modify and write better.   
    
    template<typename Type>
    bool eq(size_t n, std::vector<Type> x, Type y, Type tolerance) {
        const Type absTolerance = std::abs(tolerance);
        for(unsigned int i = 0; i < n; ++i) {
            if (std::abs(x.at(i)-y) > absTolerance) {
                return false;
            }
        }
        return true;
    }
    
    
    
}