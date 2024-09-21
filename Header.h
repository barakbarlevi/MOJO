#pragma once

#include <iostream>
#include <fstream>
// XXXX so why all the includes in source.cpp ? Arrange the includes...
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <functional>
//#include <process.h>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <mutex>
#include <condition_variable>
#include <math.h>

// XXXX IMPORTANT: is this the right place to declare the struct? is it more adequate to declare elsewhere?
// XXXX difference from typedef struct?

struct BITA_params { // XXXX name... member names...
    // XXXX edit..
    std::string tbal = "0";
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