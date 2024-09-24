#pragma once
#include <unordered_map>
#include "PredictionSupplier.h"
#include "PredSuppTrajectoryCADAC.h"   // XXXX previously: "Nakar_Trajectory.h"


class PredictionSupplierCADAC : public PredictionSupplier {   // XXXX THIS MAKES NO SENSE convetions wise. start with a capital letter or no? decide and change throught all !
    public:
    int resultUpdateBITA = 0; // xxxx delete this when not needed.

    

    // XXXX IMPORTANT: UNORDERED map the right solution? maybe write altenatives in comments and explain why picked it.
    // xxxx maybe play with the mass numbers themselves? if flight trajectory is not nice. delete this when finished xxxx
    std::unordered_map<std::string, std::string> EmptyMasses = {    // XXXX OFCOURSE CHANGE THE UNORDERED MAP.
        {"/home/barak/Source_Files/CADAC/Custom/Version7/inputOriginal.asc", "17809"},
        {"/home/barak/Source_Files/CADAC/Custom/Version7/input_Drag0p7.asc", "17809"},
        {"/home/barak/Source_Files/CADAC/Custom/Version7/input_Drag1p3.asc", "17809"}
    };

    // XXXX IMPORTANT: UNORDERED map the right solution? maybe write altenatives in comments and explain why picked it.
    // xxxx maybe play with the aerodynamic numbers themselves? if flight trajectory is not nice. delete this when finished xxxx
    std::unordered_map<std::string, std::string> Aero_decks = {    // XXXX OFCOURSE CHANGE THE UNORDERED MAP.
        {"/home/barak/Source_Files/CADAC/Custom/Version7/inputOriginal.asc", "aero_deck_SLV.asc"},
        {"/home/barak/Source_Files/CADAC/Custom/Version7/input_Drag0p7.asc", "aero_deck_SLV_Drag0p7.asc"},
        {"/home/barak/Source_Files/CADAC/Custom/Version7/input_Drag1p3.asc", "aero_deck_SLV_Drag1p3.asc"}
    };

    //home + "/Source_Files/CADAC/Custom/Version7/inputOriginal.asc",
    //home + "/Source_Files/CADAC/Custom/Version7/input_Drag0p7.asc",
    //home + "/Source_Files/CADAC/Custom/Version7/input_Drag1p3.asc"};
                                                                  

    //PredSuppTrajectoryCADAC* ptr_to_CADAC_Trajectory;  // XXXX RAW POINTER ? DELETE xxxx check when are raw / smart pointers needed and assign accordingly..

    std::shared_ptr<PredSuppTrajectoryCADAC> trajectoryCADAC;   // XXXX CHANGE NAME

    //PredictionSupplierCADAC(std::string path_to_exe, std::vector<std::string> paths_to_input_files, float dt, float dt_plot); // XXXX supplier Supplier CHANGE THROUGHOUT ALL. // XXXX don't really add anything right now.. erase.. puta comment in the "All InputFiles" and under "threadPreareInputFiles" that one can set it along with other stuff..
    PredictionSupplierCADAC(std::string path_to_exe, std::string priamryInputFile); // XXXX supplier Supplier CHANGE THROUGHOUT ALL.

    //void setTBM(std::string primaryInputFile) { this->primaryInputFile = primaryInputFile;} // XXXX CHANGE "TBM" TO SOMETHING ELSE. THORUGHT ALL CTRL+F TBM.

    virtual int updateBITA_ParamsInSupplierInput(BITA_params BITA_params);    // XXXX make sure that this is the right way of declaration. in base class virtual ... = 0 , in inhertied class this and that .... until implementation.

    // XXXX CHANGE PLACE IN DECLARATIONS (ARRANGE ALL), _ CONVENTION, TBM, MOVE TO POSIX... BUT COMMENT OUT THIS WRITING STYLE.
    std::thread threadupdateBITA_ParamsInSupplierInput(BITA_params _BITA_params) {
        //return std::thread([=]{resultUpdateBITA = updateBITA_ParamsInSupplierInput(_BITA_params);});
        return std::thread([=]{updateBITA_ParamsInSupplierInput(_BITA_params);});
    }


    virtual int prepareInputFiles();

    // XXXX CHANGE PLACE IN DECLARATIONS (ARRANGE ALL), _ CONVENTION, TBM, MOVE TO POSIX... BUT COMMENT OUT THIS WRITING STYLE.
    // XXXX EXPLAIN IN ALL PLACES ABOUT THIS WRITING WITH THE LAMDA CAPTURE. QUOTE FROM STACK EXCHANGE.
    std::thread threadPrepareInputFiles() {
        return std::thread([=] {prepareInputFiles();});
    }

    //virtual void runSupplierOnce();
    virtual int runSupplierOnce();

    
    std::thread threadRunSupplierOnce() {
        return std::thread([=] { runSupplierOnce(); });
    }

    

    // xxxx delete if unused
    /*
    std::thread threadRunSupplierOnce() {
       
        return std::thread([=] { runSupplierOnce(); });
    }
    */
    



    protected:  // xxxx
    


};