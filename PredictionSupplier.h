#pragma once
#include "BITA_params.h"


class PredictionSupplier {
    
    public: 
    
    PredictionSupplier(std::string path_to_exe, std::string priamryInputFile);    // XXXX names xxxx dt, dt_plot don't add anything right now.. erase..
    virtual int prepareInputFiles() = 0;    // XXXX name?
    virtual int runSupplierOnce() = 0;  // XXXX name?
    virtual int updateBITA_ParamsInSupplierInput(BITA_params BITA_params) = 0;    // XXXX

    protected:

    std::string primaryInputFile;   // XXXX OFC NOT TBM, CHANGE
    std::string path_to_exe = "";   // XXXX no exe in linux, change name
};