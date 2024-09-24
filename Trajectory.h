#pragma once
#include <mutex>
#include <condition_variable>
#include "utils.h"
#include "BITA_params.h"

// xxxx all needed here?




class Trajectory {
    
    public:

    std::mutex syncDetectSetBITA_mutex;
    std::condition_variable syncDetectSetBITA_cv;
    bool syncDetectSetBITA_ready = false;
    bool reachedHdetection = false;

    //std::mutex syncPlotAndDecision_mutex;
    //std::condition_variable syncPlotAndDecision_cv;
    bool finishedplotting2 = false; // xxxx name

    //SyncObj synchObj; // xxxx


    std::string loadPath;   // XXXX Make sure all variable names areAlright
    int FirstLineOfNumericData_ = 0; // XXXX explain in good english that's different for each simulation etc.   XXXX MAKE THIS INSIDE CONSTRUCTOR ! cant cnstrt without knowing it !
    std::string KML_path;
    std::string SingleCoordsLine;   // XXXX So many publics.. organize...   
    std::string color =  "ff00AAff";
    std::string scale =  "0.4";
    std::vector<std::string> data;  // XXXX nothing is left protected :(
    BITA_params _BITA_Params;   // XXXX Make sure all variable names _areOK. stackexchange on when to put _ before it at all. also cobra
    unsigned int currentDetectionIndex = 0;    // Row index inside the file containing all information about the trajectory.
    bool finishedPlotting = false;
    
    //Trajectory(std::string loadPath, std::string KML_path, std::string in, std::string pl, std::string mo, int FirstLineOfNumericData);  // XXXX ADD CONSTS? WHEN TO ADD CONSTS? WRITE IN COMMENT TO CLARFIYIF NEEDED
    Trajectory(std::string loadPath, std::string kmlPath);  // XXXX ADD CONSTS? WHEN TO ADD CONSTS? WRITE IN COMMENT TO CLARFIYIF NEEDED
    ~Trajectory() = default;    // XXXX COMPLETE IF THERE's memory allocation needed to be deleted. WHAT'S THE MEANING OF default? stackoverflow. write explicitly in a comment

    //virtual void readInput(bool isDetection);  // XXXX Addressing it here as simply reading a file that's ready already. It would change in case of a sensor sending data in real time XXXX ENGLISH    XXXX IMPOrtant: why is this method virtual? can the virtual word be deleted and it'll work fine? if not, what's the explanation?
    virtual int readInput(bool isDetection);  // XXXX Addressing it here as simply reading a file that's ready already. It would change in case of a sensor sending data in real time XXXX ENGLISH    XXXX IMPOrtant: why is this method virtual? can the virtual word be deleted and it'll work fine? if not, what's the explanation?

    // XXXX CHANGE TO POSIX. MENTION IN COMMENT IT CAN ALSO BE done like this, and give reference to the stackoverflow thread giving this writing method. explain a bit on the lambda and the  capture.
    std::thread threadReadInput(bool isDetection) {
     
        return std::thread([=] {readInput(isDetection);});
    }


    //virtual int PlotTrajectoryCoordByCoord(int indexJump) = 0;  // XXXX Arrange so that every class is written nicely... with vars and methods.. not a mess. that it will look good to the eye

    /*
    std::thread threadPlotTrajCoordByCoord(int indexJump) {
        return std::thread([=]{PlotTrajectoryCoordByCoord(indexJump);});
    }
    */

    void clearGE_lock() {
        std::string COMMAND1 = "rm ~/.googleearth/instance-running-lock";
        std::system(COMMAND1.c_str());
    }

    void loadKML_ToGE(std::string KML) { 
        //std::string COMMAND1 = "rm ~/.googleearth/instance-running-lock";
        //std::system(COMMAND1.c_str());
      
        std::string COMMAND3 = "google-earth-pro " + KML + " &";
        std::system(COMMAND3.c_str());
        }
            // XXXX looks like it might be non-modern implementation (.cstr()...). ALSO: EXPLAIN WHAT IS GE

    std::thread threadLoadKML_ToGE(std::string KML) {   // XXXX STILL NEEDED ? DELETE ? OTHER METHODS / CODE THAT BECAME NOT NEEDED ?
        return std::thread([=]{loadKML_ToGE(KML);});
    }

    std::thread threadclearGE_lock() {
        return std::thread([=]{clearGE_lock();});
    }
    

    //void plotTrajectoryAtOnce(Trajectory* Trajectory, std::string KML, int indexJump, int currentNumbebrOfSuppliers, int CollectorSize, float StyleScale);  // XXXX if really implementing like this, should it be raw pointer or smart pointer? ownership etc xxxx after implementing plotcollectorallatonce, go over this one and see if all arguments are really needed. looks like this is only for 1 traj, whose kml is already known, and the rest of the args too. xxxx need to have ownership on the object? need to pass smart pointer?
    /*
    void plotTrajectoryAtOnce(std::string KML, int indexJump, int currentNumbebrOfSuppliers, int CollectorSize, float StyleScale) {
        utils::kmlInsertEntireTrajectory(this, KML, indexJump, currentNumbebrOfSuppliers, CollectorSize, StyleScale);    //  XXXX
        utils::kmlInsertOneNetworkLink("Secondary_Controller.kml",KML);
    }    //  XXXX    A FIGURE EXPLAINING THE LAYOUT: MAIN TRAJECTORY SENSOR, SUPPLIER, COLLOCTOR, ETC. can be done with screenshop from ge.    XXXX delete if not needed here
    */


    //void plotTrajectoryCoordByCoord(Trajectory* Trajectory, int indexJump);  // XXXX if really implementing like this, should it be raw pointer or smart pointer? ownership etc xxxx delete if needed
    
    //void plotTrajectoryCoordByCoord(int indexJump, int delayms);
    
    void appendTrajectoryToKML(int indexJump, int currentSupplierNumber, int CollectorSize, bool isCollector);
    void appendTrajectoryToKML(int indexJump, int delayms);
   
   // xxxx not in use at all ? if so, delete
    std::thread threadpPlotTrajectoryAtOnce(int indexJump, int currentNumberOfSuppliers, int CollectorSize, bool isCollector) {
        if(!isCollector) {
            utils::kmlInit_href(this->KML_path, this->KML_path, this->color);

        }   // XXXX for simplicity ZMANIT assuming only plotting detections coord-by-coord. it's less meaningful for the prediction suppliers and i dont wanna mess up the code with tons of passed arguments or LEZAMZEM the option of plotting coord by coord to only detections and not predictions.
        
        return std::thread([=]{appendTrajectoryToKML(indexJump, currentNumberOfSuppliers, CollectorSize, isCollector);});
    }


    // xxxx why the hell is this method not in SensorTrajectoy ? I moved this piece as is to SensorTrajectory.h and commented out because I don't compile ATM. after compiling check. should be there by reason..
    std::thread threadplotTrajectoryCoordByCoord(int indexJump, int delayms) {
        utils::kmlInit_href(this->KML_path, this->KML_path, this->color);
        return std::thread([=]{appendTrajectoryToKML(indexJump, delayms);});
    }   // XXXX as of now i'm taking into consideration only 1 detection and predictofrs for it. that's why it has a defined style, written in Source.cpp. mention that if anybody wants to expand to multiple detections inspection simultaneously, then can pretty easily modify it. but not right now.


    virtual void setSingleCoordsLine() = 0;  // XXXX this is what's really different for every trajectory. not inserting into kml...







    // xxxx delete when finished 
    void *hello(void)
    {
        std::cout << "Hello, world!" << std::endl;
        return 0;
    }

    static void *hello_helper(void *context)
    {
        return ((Trajectory *)context)->hello();
    }




    /*
    virtual int kmlAppendOneCoord(std::string KML, std::string styleID, std::string SensorOrSupplier); // XXXX Kinda looks like bad practive to have these arguments. see if can modify and write better.
    virtual int kmlInsertOneStyle(std::string KML, std::string styleID, std::string color, std::string scale);
    //virtual int kmlInsertEntireTrajectory(std::string KML, int indexJump, int currentNumbebrOfSuppliers, int CollectorSize, float StyleScale) = 0;    //  XXXX    A FIGURE EXPLAINING THE LAYOUT: MAIN TRAJECTORY SENSOR, SUPPLIER, COLLOCTOR, ETC. can be done with screenshop from ge.    XXXX delete if not needed here
    */
    
    protected:
    
    

};
