#include "PredictionSupplier.h"

// XXXX names xxxx make order
PredictionSupplier::PredictionSupplier(std::string path_to_exe, std::string primaryInputFile) {
    this->path_to_exe = path_to_exe;    // xxxx add _
    this->primaryInputFile = primaryInputFile;  // xxxx add _
    //this->dt = dt;    // XXXX don't really add anything right now.. erase.. puta comment in the "All InputFiles" and under "threadPreareInputFiles" that one can set it along with other stuff..
    //this-> dt_plot = dt_plot;// XXXX don't really add anything right now.. erase.. puta comment in the "All InputFiles" and under "threadPreareInputFiles" that one can set it along with other stuff..   
}