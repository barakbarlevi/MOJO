#pragma once

#include "PredictionSupplier.h"
#include <unordered_map>
#include "PredSuppTrajectoryCADAC.h"   // XXXX previously: "Nakar_Trajectory.h"
#include <fstream>
#include <cstring>

extern std::mutex ChangeFiles_Mutex;    // XXXX IMPORTANT: UNDERSTAND WHY THE HELL is this mutex needed? on this machine also? XXXX give proper name. XXXX Explain in comment why extern. find in stackexchange examples of using extern for mutex.

extern bool gSyncOutputReading_ready;
extern bool loopMaintainer;  // xxxx needed?
extern int readOnce;

extern std::mutex mux;
extern std::condition_variable cv;  
extern bool completed[3];
extern void signalCompletionOfT1();
extern void signalCompletionOfT2();
extern void signalCompletionOfT3();
extern void waitForCompletionOfT1();
extern void waitForCompletionOfT2();
extern void waitForCompletionOfT3(); 

extern std::binary_semaphore BS1;
extern std::binary_semaphore BS2;
extern std::binary_semaphore BS3;

class PredictionSupplierCADAC : public PredictionSupplier {   // XXXX THIS MAKES NO SENSE convetions wise. start with a capital letter or no? decide and change throught all !
    public:
    int resultUpdateBITA = 0; // xxxx delete this when not needed.

    /*
    bool gSyncOutputReading_ready = false;
    bool loopMaintainer = true;  // xxxx needed?
    */

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

    //virtual void updateBITA_ParamsInSupplierInput(BITA_params BITA_params);    // XXXX make sure that this is the right way of declaration. in base class virtual ... = 0 , in inhertied class this and that .... until implementation.
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