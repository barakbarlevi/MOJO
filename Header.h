#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <functional>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <mutex>
#include <condition_variable>
#include <math.h>



struct BITA_params { // XXXX name... member names...
    // XXXX edit..
    std::string BITA_time = "0";
    std::string massbal = "0";
    //std::string sxbal = "0";    // xxxx remove this...
    //std::string sybal = "0";
    std::string height = "0";
    std::string vbal = "0";
    std::string gamalbal = "0";
    std::string azimlbal = "0";
    //std::string dvbal = "0";
    //std::string etabal = "0";
    //std::string phibal = "0";
    // xxxx keep only what's needed
    std::string lat = "0";
    std::string lon = "0";
};

enum predictionSuppliers {
    CADAC,
    // Add more prediction suppliers (simulations) here
};