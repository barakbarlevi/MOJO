#pragma once

#include "Header.h"

class PredictionSupplier {
    
    public: 
    


    //PredictionSupplier(std::string path_to_exe, std::vector<std::string> paths_to_input_files, float dt, float dt_plot);    // XXXX names...
    PredictionSupplier(std::string path_to_exe, std::string priamryInputFile);    // XXXX names xxxx dt, dt_plot don't add anything right now.. erase..

    virtual int prepareInputFiles() = 0;    // XXXX name?
    
    //virtual void runSupplierOnce() = 0;  // XXXX name?
    virtual int runSupplierOnce() = 0;  // XXXX name?
    
    virtual int updateBITA_ParamsInSupplierInput(BITA_params BITA_params) = 0;    // XXXX

    protected:
    std::string primaryInputFile;   // XXXX OFC NOT TBM, CHANGE
    std::string path_to_exe = "";   // XXXX no exe in linux, change name
    //std::vector<std::string> paths_to_input_files;  // xxxx name xxxx by convention xxxx decide whether to leave commented out or completely remove

    
    
    //float dt = 0;   // XXXX int init way  // xxxx doesn't add anything.. erase..
    //float dt_plot = 0;  // XXXX int init way // xxxx doesn't add anything.. erase..

};