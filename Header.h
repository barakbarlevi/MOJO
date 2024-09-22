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
    std::string BITA_mass = "0";
    std::string BITA_height = "0";
    std::string BITA_speed = "0";
    std::string BITA_flightPath = "0";
    std::string BITA_heading = "0";
    std::string BITA_lat = "0";
    std::string BITA_lon = "0";
};

enum predictionSuppliers {
    CADAC,
    // Add more prediction suppliers (simulations) here
};