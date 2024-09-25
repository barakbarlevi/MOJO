#pragma once
#include <mutex>
#include <condition_variable>
#include "utils.h"
#include "BITA_params.h"

class Trajectory {
    
    public:
    // bool reachedheightFirstDetection_ = false;
    // bool finishedPlotting = false; // xxxx make order in these two and change names. in the new name, include _ in the end
    // bool finishedplotting2 = false; // xxxx name
    // int FirstLineOfNumericData_; // Output files for different simulations vary in their structure. The program reads the file line by line. This is the index of the first line containing numerical values to parse.
    // std::string loadPath_; // The path from which trajectory data is loaded.
    // std::string kmlPath_; // The path to the .kml file that will contain the trajectory data to visualize in Google Earth.
    // std::string SingleCoordsLine_; // Hold the momentary (Lat, Lon, Height) coordinate of the trajectory.
    // std::string colorInGE_ =  "ff00AAff"; // Default color for Google Earth display
    // std::string scale_ =  "0.4"; // Default scale for Google Earth display
    // std::vector<std::string> data_;  // Contains any data the relevant for the trajectory: time, position, flight path and heading angles, etc.
    // BITA_params BITA_Params_; // Momentary state vector, containing any data that can be measured for the detected target: position, velocity, heading etc.
     
    Trajectory(std::string loadPath, std::string kmlPath);
    ~Trajectory() = default;
    virtual int readInput(bool isDetection); // Read trajectory input. 
    virtual void setSingleCoordsLine() = 0; // Set the momentary (Lat, Lon, Height) coordinate of the trajectory.
    
    bool getReachedheightFirstDetection_() {return reachedheightFirstDetection_;}
    bool getFinishedPlotting() {return finishedPlotting;}
    bool getFinishedPlotting2() {return finishedplotting2;}
    BITA_params getBITA_Params() {return BITA_Params_;}

    void setFinishedPlotting(const bool& value) {finishedPlotting = value;}
    void setFinishedPlotting2(const bool& value) {finishedplotting2 = value;}
    void setReachedheightFirstDetection(const bool& value) {reachedheightFirstDetection_ = value;}

    protected:
    std::string loadPath_; // The path from which trajectory data is loaded.
    std::string kmlPath_; // The path to the .kml file that will contain the trajectory data to visualize in Google Earth.
    std::string SingleCoordsLine_; // Hold the momentary (Lat, Lon, Height) coordinate of the trajectory.
    std::string colorInGE_ =  "ff00AAff"; // Default color for Google Earth display
    std::string scale_ =  "0.4"; // Default scale for Google Earth display
    std::vector<std::string> data_;  // Contains any data the relevant for the trajectory: time, position, flight path and heading angles, etc.
    BITA_params BITA_Params_; // Momentary state vector, containing any data that can be measured for the detected target: position, velocity, heading etc.
    int FirstLineOfNumericData_; // Output files for different simulations vary in their structure. The program reads the file line by line. This is the index of the first line containing numerical values to parse.

    private:
    bool reachedheightFirstDetection_ = false;
    bool finishedPlotting = false; // xxxx make order in these two and change names. in the new name, include _ in the end xxxx amd see what methods are still relevant and what not
    bool finishedplotting2 = false; // xxxx name
    
    

};
