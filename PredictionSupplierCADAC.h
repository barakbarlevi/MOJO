#pragma once
#include <unordered_map>
#include "PredictionSupplier.h"
#include "PredSuppTrajectoryCADAC.h"


class PredictionSupplierCADAC : public PredictionSupplier {

    public:  

    PredictionSupplierCADAC(std::string path_to_exe, std::string priamryInputFile, std::string pathCADAC);
    ~PredictionSupplierCADAC() = default;
    
    
    // std::unordered_map<std::string, std::string> EmptyMasses_ = {    // XXXX OFCOURSE CHANGE THE UNORDERED MAP.
    //     {"/home/barak/Source_Files/CADAC/Custom/Version7/inputOriginal.asc", "17809"},
    //     {"/home/barak/Source_Files/CADAC/Custom/Version7/input_Drag0p7.asc", "17809"},
    //     {"/home/barak/Source_Files/CADAC/Custom/Version7/input_Drag1p3.asc", "17809"}
    // };

    // std::unordered_map<std::string, std::string> Aero_decks_ = {    // XXXX OFCOURSE CHANGE THE UNORDERED MAP.
    //     {"/home/barak/Source_Files/CADAC/Custom/Version7/inputOriginal.asc", "aero_deck_SLV.asc"},
    //     {"/home/barak/Source_Files/CADAC/Custom/Version7/input_Drag0p7.asc", "aero_deck_SLV_Drag0p7.asc"},
    //     {"/home/barak/Source_Files/CADAC/Custom/Version7/input_Drag1p3.asc", "aero_deck_SLV_Drag1p3.asc"}
    // };

    

    virtual int updateBITA_ParamsInSupplierInput(BITA_params BITA_params);

    std::thread threadupdateBITA_ParamsInSupplierInput(BITA_params _BITA_params) {
        return std::thread([=]{updateBITA_ParamsInSupplierInput(_BITA_params);});
    }

    virtual int runSupplierOnce();

    std::thread threadRunSupplierOnce() {
        return std::thread([=] { runSupplierOnce(); });
    }

    std::shared_ptr<PredSuppTrajectoryCADAC> getTrajectoryCADAC() const {return trajectoryCADAC_;}
    

    //std::shared_ptr<PredSuppTrajectoryCADAC> trajectoryCADAC_;  

    protected:

    private:
    std::string pathCADAC_;
    std::unordered_map<std::string, std::string> EmptyMasses_;
    std::unordered_map<std::string, std::string> Aero_decks_;

    std::shared_ptr<PredSuppTrajectoryCADAC> trajectoryCADAC_;  

};