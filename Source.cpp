/******************************************************************************
MOJO - a ballistic trajectory analyzer
 
Purpose: Analyze live detection data of a single ballistic target, comparing it
to expected behavior from 6-DOF simulation outputs.
 
Description: The program receives detections live over the network. Detection 
data may contain any information that a generic sensor could be able to 
measure, such as estimated position, velocity etc. Starting at a pre-
determined point along the trajectory, the program will start executing a
series of 6-DOF simulation runs for an array of potential airframe models,
with the initial conditions being passed to it at the moment, from the tracked
data. The 6-DOF is a separate project. The simulation's results for all
airframe models will be stored. The program will continue to receive the live
ballistic target data until it hits the ground, and will be able generate
simulation outputs during the entire flight time. As soon as the first set of
simulation output is ready, the program will run a backgroud process to compare
target detections and simulation output against each other. This background
process can implement any logic coded into it, its purpose to send a
notification when some programmer-defined conditions have been met.

Input: The program isn't provided input parameters. It listens on a socket, and
handles data arriving on it.

Output: The program doesn't return a value other than '0' for success. If 
specified conditions relating the target track data and the simulation output
are met, a notification is sent.
   
Notes: 
1. As target detections are simulation output themselves, the point at
which calculations on the live 6-DOF executions start can be the detected
target in its starting conditions. This doesn't represnt a real life case for
ballisitic targets, since first detection is expected to occur after the object
has gained height. Parameter 'detectionHeight' of type double is set for this
reason.
2. In this program's context, the term 'Prediction' is referring to any type
of simulation's output, that describes the progression of a modeled airframe
motion through the air for the momentary initial conditions. The term
'Supplier' refers to the simulation's itself. In this program, the sole
supplier of predictions is the CADAC++ simulation, be anyone can add other
simulations as source of information, with adjusting to meet their required
metadata structure.
3. An effort was made to work according to coding conventions given in
https://geosoft.no/development/cppstyle.html.

  
******************************************************************************/

// xxxx put big intro text in grammarly
// XXXX go though all public / protected / private sections and decide on the best declarations.
// xxxx turn suppliersCollectorsVector into just a vector.. no actual need to create a class for it.
// xxxx "int" i ? ++i or i++ ? xxxx always size_t? maybe unsigned?
// xxxx what abouth factory design pattern? how can it be implemented in mojo?

#include "Header.h"

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
#include <mutex>
#include <memory>
#include <filesystem>
#include <pthread.h>

#include "PredictionSupplierCADAC.h"
#include "SensorTrajectoryCADAC.h"
#include "PredSuppTrajectoryCADAC.h"
#include "SuppliersCollector.h"
#include "utils.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <semaphore>


#include "SuppliersCollectorsVector.h"
#include "DecisionMaker.h"
#include "SyncDataArrivalAndPredicting.h"
#include "X11_window.h"



bool check{false};  // xxxx for checking, delete



int main(int argc, char *argv[])
{ 
    (void)argc;
    (void)argv;
  
    pthread_t thread1;
    SyncDataArrivalAndPredicting *syncSingleton = new SyncDataArrivalAndPredicting();
    pthread_mutex_init(&syncSingleton->condition_lock_color, NULL);
    pthread_cond_init(&syncSingleton->condition_variable_color, NULL);
    pthread_mutex_init(&syncSingleton->condition_lock_finished, NULL);
    pthread_cond_init(&syncSingleton->condition_variable_finished, NULL);
    pthread_create( &thread1, NULL, init15, (void*) syncSingleton);
    
    //SyncDataArrivalAndPredicting *syncSingleton = new SyncDataArrivalAndPredicting();




    char* homeENV = getenv("HOME");
    std::stringstream ss;
    ss.str(homeENV);
    std::string home = ss.str();


    std::array<std::string, 2> detectedTrajectories = {home + "/Source_Files/CADAC/Custom/Version7/Vandenberg_az180_hUP",
                                                       home + "/Source_Files/CADAC/Custom/Version7/Vandenberg_az180"}; // Absolute / relative paths of detection plots/tracks/simulation data. xxxx absolute path looks like crap FIX change to relative path    

    std::vector<predictionSuppliers> predictionSuppliers = {CADAC, CADAC, CADAC};

    std::vector<std::string> currentCollectorPriamryInputFiles = {home + "/Source_Files/CADAC/Custom/Version7/inputOriginal.asc",
                                                                  home + "/Source_Files/CADAC/Custom/Version7/input_Drag0p7.asc",
                                                                  home + "/Source_Files/CADAC/Custom/Version7/input_Drag1p3.asc"}; // Absolute / relative paths of detection plots/tracks/simulation data. xxxx absolute path looks like crap FIX change to relative path. xxxx should be a vector in order to allow predictors modification in flight xxxx english

    std::vector<std::string> currentCollectorExecutables = {home + "/Source_Files/CADAC/Custom/Version7/SIX_DOF",
                                                            home + "/Source_Files/CADAC/Custom/Version7/SIX_DOF",
                                                            home + "/Source_Files/CADAC/Custom/Version7/SIX_DOF"}; // Absolute / relative paths of detection plots/tracks/simulation data. xxxx absolute path looks like crap FIX change to relative path. xxxx should be a vector in order to allow predictors modification in flight xxxx english

    std::vector<std::string> currentCollectorLoadPaths = {home + "/Source_Files/CADAC/Custom/Version7/cadacOutputVDXVDX.asc",
                                                          home + "/Source_Files/CADAC/Custom/Version7/cadacOutputVDXVDX.asc",
                                                          home + "/Source_Files/CADAC/Custom/Version7/cadacOutputVDXVDX.asc"}; // Absolute / relative paths of detection plots/tracks/simulation data. xxxx absolute path looks like crap FIX change to relative path. xxxx should be a vector in order to allow predictors modification in flight xxxx english xxxx explain that this also is a vector for the same reasons

    utils::kmlInitPrimaryController();
    utils::kmlInitSecondaryController();

// Plot at once one or more trajectories to Google Earth.
#if 0
    for (unsigned int i = 0; i < detectedTrajectories.size(); i++)
    { // XXXX what is the best decleration for i ? auto? so many others i've seen?

        std::string detectionKML = "Detection" + std::to_string(i) + ".kml";    // XXXX names... to_string... XXXX THIS IS A TRICKY LINE BECAUSE i should mention that anybody has their own path. plus i'm showing here mine... FIX
        SensorTrajectoryCADAC Sensor(detectedTrajectories.at(i), detectionKML); // XXXX 3xNotspecified? what are these, deleteable? xxxx explain why 1 etc.
        Sensor.readInput();                                                     // XXXX Names.. loadPath the best ? how about dataLoadPath?

        /* there's noise here. basically, sensor trajectories from the bank shouldn't have different colors.
           when trying to properly write the pthreads implementation, it's important because
           class methods may beed parameters passed and if u can store everything u need for the plotting inside
           the class as members, then u probably can use this-> and don't need an assistant struct.
           so continuing with the second part of main, then will pass everything to pthreads.
        */

        std::thread THREAD = Sensor.threadpPlotTrajectoryAtOnce(5, 0, 0, 0); // XXXX THREAD name differently
        THREAD.join();                                                        // XXXX THREAD name differently and move to POSIX

        // pthread_t t;
        // pthread_create(&t, NULL, &Trajectory::hello_helper , &Sensor);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // xxxx why is this needed..?
    }

#endif

// Plot one trajectory gradually - the trajectory is pre-calculated offline and stored as a whole in a file.
// xxxx this was commented since started dealing with RT. test make sure this and the one before still work before writing it in the document that they exist as options.
#if 0

    // XXXX move ALL declerations up
    std::string detectionKML = "Detection.kml";        // XXXX
    std::vector<std::string> collectorsKMLs_List = {}; // xxxx not sure about this line. CHECK WITH WORK.

    SensorTrajectoryCADAC trajectoryFromSensor(detectedTrajectories.at(0), detectionKML); // XXXX Make sure about position units.. km / m... write in nice comment..
    trajectoryFromSensor.readInput(true);
    std::thread pltThread = trajectoryFromSensor.threadplotTrajectoryCoordByCoord(5, 200); // XXXX give better name. XXXX parameterize

#endif

// Plot one trajectory gradually - recieve Lat, Lon, Alt data live via TCP/IP.
#if 1

    // XXXX move ALL declerations up
    std::string detectionKML = "DetectionRT.kml";        // XXXX
    std::vector<std::string> collectorsKMLs_List = {}; // xxxx not sure about this line. CHECK WITH WORK.

    // xxxx this has to be CADAC and not general trajectory because we need to know how to parse information.
    SensorTrajectoryCADAC trajectoryFromSensor("RT", detectionKML); // XXXX Make sure about position units.. km / m... write in nice comment..
    
    //trajectoryFromSensor.readInput(true);

    //SyncDataArrivalAndPredicting* syncSingleton;
    
    //SyncDataArrivalAndPredicting *syncSingleton = new SyncDataArrivalAndPredicting();
    
    //std::shared_ptr<SyncDataArrivalAndPredicting> syncSingleton = std::make_shared<SyncDataArrivalAndPredicting>();
    std::thread pltThread = trajectoryFromSensor.threadPlotDataFromRT(syncSingleton); // XXXX give better name. XXXX parameterize

#endif




#if 1

    syncSingleton->WaitForFirstMsg();

    std::cout << "Started plotting inspected trajectory\n" << std::endl; // xxxx is this the right place ? XXXX better english xxxx google earth has to be opened with primarykml loaded

    // XXXX Operate / do something when reaching a specific height on ascent:  

    float detectionHeight(15000); // XXXX best way to init ints / floats? they have a name... with dobule, long etc.. these types xxxx units - meter ?
    //float detectionHeight(500); // XXXX best way to init ints / floats? they have a name... with dobule, long etc.. these types xxxx units - meter ?

    // xxxx in this case we are operating when reaching a certain height. on its face, we could have passed detectionHeight to threadplotTrajectoryCoordByCoord(5, 200, H),
    // assign bita_params.height there, and wait for reaching it. but what if we want to operate based on any other feature of bita_params? pass that too? so i decided
    // that i do want to update bita params every time i can until reaching that condition. or maybe, put a condition in there that is changeable.
    // and in main, it will only look like conditionIsMet, waiting for a notification on it. but then i would be forced to check on it every time even after
    // it's met! and it's a waste. so maybe i do want a seperate thread updating BITA but in a way that currentrowindex is safe xxxx write all of these considerations
    // in order to explain to the reader why i did what i did.
    // just a mutex doesn't let setbitaparams run. it doesn't get released in plottrahcoordbycoord.
    
    // xxxx could have used a semaphore instead?
    while ((std::stof(trajectoryFromSensor._BITA_Params.height) < detectionHeight) && (trajectoryFromSensor.get_vVertical() <= 0)) // XXXX height not a good name. nor vVertical. explain on coordinate system. XXXX nor detectionHeight. specify it's FIRST detect. be clear
    {
        // https://stackoverflow.com/questions/12551341/when-is-a-condition-variable-needed-isnt-a-mutex-enough
        // let's make this the consumer 1 xxxx

        std::unique_lock<std::mutex> ul(trajectoryFromSensor.syncDetectSetBITA_mutex);
        trajectoryFromSensor.syncDetectSetBITA_cv.wait(ul, [&](){ return trajectoryFromSensor.syncDetectSetBITA_ready; }); // xxxx here i learned in the tough way the imprtance of capturing by reference and not by value ENGLISH. put it?

        trajectoryFromSensor.setBITA_Params();                                            // xxxx i generally don't wanna wait till setting bita params is done when plotting. it's very important to plot in relevant time. what if setting up bita params will take a very long time? the target doesn't wait for me.
        std::cout << "height: " << trajectoryFromSensor._BITA_Params.height << std::endl; // XXXX can't keep "height". change.

        trajectoryFromSensor.syncDetectSetBITA_ready = false;
        ul.unlock();

        trajectoryFromSensor.syncDetectSetBITA_cv.notify_one();
        ul.lock();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::cout << "Reached " << detectionHeight << "[m], at currentRowIndex: " << trajectoryFromSensor.currentRowIndex << std::endl; // XXXX choose what to cout, only necessary.
    float detectionTime = std::stof(trajectoryFromSensor._BITA_Params.tbal);
    trajectoryFromSensor.reachedHdetection = true;

#endif




//pltThread.join(); // XXXX posix XXXX name XXXX explain why here, explain also for all pthreads.



#if 1
    //int suppliersCollectorsVectorIndex(0);                                      // XXXX best init way ? bring up.
    //std::vector<std::shared_ptr<SuppliersCollector>> suppliersCollectorsVector; // XXXX SuppliersCollectorsVector is this the right name? plural / singular wise. XXXX bring up. xxxx should it be a vector or is it constant size? go through all vector definitions in the code and decide.
    
    SuppliersCollectorsVector suppliersCollectorsVector;
    DecisionMaker decisionMaker(&suppliersCollectorsVector, &trajectoryFromSensor);    // xxxx no faults in this line?
    //while (trajectoryFromSensor.currentRowIndex <= trajectoryFromSensor.data.size()) // as long as ballistic motion still takes place. loop on collectors xxxx english
    do
    {
        std::shared_ptr<SuppliersCollector> currentCollector = std::make_shared<SuppliersCollector>(std::stof(trajectoryFromSensor._BITA_Params.tbal) - detectionTime);
        suppliersCollectorsVector.suppliersCollectorsVector_.push_back(currentCollector);
        //suppliersCollectorsVector.push_back(currentCollector);

        currentCollector->collectorKML_ = "Collector" + std::to_string(suppliersCollectorsVector.currentNumOfSuppliersCollectors_) + ".kml";

        for (size_t i = 0; i < currentCollectorPriamryInputFiles.size(); ++i) // loop on number of current suppliers for the current collector xxxx english
        {                                                                     

            switch (predictionSuppliers.at(i))
            {
                case CADAC:
                {
                    std::shared_ptr<PredictionSupplierCADAC> predictionSupplierCADAC = std::make_shared<PredictionSupplierCADAC>(currentCollectorExecutables.at(i), currentCollectorPriamryInputFiles.at(i)); // xxxx this is the point where polymorphism has to be reduced to monomorphism ENGLISH. need to understand what kind of derived class dealing with, because can't use the pure virtual functions of PredictionSupplier.h. should mention how to create a distinguishment between different types of suppliers, cause now it's not really given. maybe have seperate intervals of i for each simulation. for instance: i=0 to 3 is CADAC. i = 4 to 7 is something else. i = 8 to 10 is third etc.

                    // xxxx Leave this here in comment so ppl will know that it exists and what's its purpose
                    // std::thread T0 = unq_ptr_to_PredictionSupplierCADAC->threadPrepareInputFiles();   // XXXX wrinte as a comment: this would have been the place to any supplementary input files editing. for example: change simulation dt or any other type of simulation-depended necessary editing. xxxx english. i don't need it right now so i'm not implementing it. // XXXX name T0 to indicative. POSIX
                    
                    

                    // Threaded version, delete if not needed xxxx
                    //std::thread T1 = predictionSupplierCADAC->threadupdateBITA_ParamsInSupplierInput(trajectoryFromSensor.getBITA_Params()); // XXXX names XXXX POSIX xxxx mention: way of updating bita params in input files may vary.
                    //T1.join();                                                                                                            // XXXX name XXXX POSIX                

                    

                    /*******************************************
                    int first, second, third;

                    first = predictionSupplierCADAC->updateBITA_ParamsInSupplierInput(trajectoryFromSensor.getBITA_Params()); // XXXX names XXXX POSIX xxxx mention: way of updating bita params in input files may vary.
                    first ++;
                                                                                                                  // XXXX name XXXX POSIX                
                    second = predictionSupplierCADAC->runSupplierOnce(); // XXXX POSIX XXXX names xxxx no matter what, this will have to include process sych. cuz on the one hand displaying radar data and on the other hand running simulation and plotting it. i think it's a good first step to let it run on a seperate thread. xxxx mention: way of running once may vary.
                    second = first + 1;

                    predictionSupplierCADAC->trajectoryCADAC = std::make_shared<PredSuppTrajectoryCADAC>(currentCollectorLoadPaths.at(i), currentCollector->collectorKML_);
                    
                    third = second;
                    third = predictionSupplierCADAC->trajectoryCADAC->readInput(false);
                    third = second + 1;                    

                    // Had the lines above not been synchronized in some way, a situation where the same input file is read would occur from time to time.
                    // A visualization of this is given in the document, where one could see trajectories completely overlap, since they're identicle.
                    // They could be two subsequent trajectories inside the same supplier, or within two suppliers, one being the last of the preceeding supplier
                    // and the other one being the first of the succeeding supplier.
                    // I had a hard time trying to understand why this synchronization is needed. Presumably, the method running the supplier once should block the current program
                    // from executing the line for input reading, since the method uses std::system() which is blocking. I tweeked the code a bit and witness it does block.
                    // I first assumed that if the call to read input only happens after the trajectory simulation had ended and generated a new output file, there should be no problem.
                    // When debugging, I saw that the overlapping trajectories contain the exact same data, which means my assumption above wasn't right.
                    // I also tried to rule out sequential inconsistency due to debugger optimization by using the functions return values.
                    // I assumed sequential consistency that didn't exist.
                    // My estimation of this situation is based on this post about sequential consistency https://stackoverflow.com/questions/38425920/memory-model-in-c-sequential-consistency-and-atomicity 
                    // and specifically the statement that: "If you just use plain non-atomic operations, or relaxed atomics, and no mutexes, then sequential consistency is not guaranteed."                
                    // At first I tried to run the lines as threads and use join() on the in a sequential order. That performed exactly like not threadding at all !
                    // It's in my assumption that the structure of joinning a thread right after instantiating it is meanningless and interperted as if it was written
                    // in exactly in the above form. 
                    // I then tried to overcome the overlapping trajectories issue by synchronizing the simulation run, which ends in an output file being written,
                    // and its reading. I think i got it right using a conditional variable, see https://stackoverflow.com/questions/78794995/synchronizing-simulation-output-generation-and-reading-using-producer-and-consum?noredirect=1#comment138922877_78794995:
                    
                    std::thread T1 = predictionSupplierCADAC->threadupdateBITA_ParamsInSupplierInput(trajectoryFromSensor.getBITA_Params()); // XXXX names XXXX POSIX xxxx mention: way of updating bita params in input files may vary.
                    T1.join();

                    std::thread T2 = predictionSupplierCADAC->threadRunSupplierOnce();
                    
                    predictionSupplierCADAC->trajectoryCADAC = std::make_shared<PredSuppTrajectoryCADAC>(currentCollectorLoadPaths.at(i), currentCollector->collectorKML_);

                    std::thread T3 = predictionSupplierCADAC->trajectoryCADAC->threadReadInput(false);
       
                    T2.join();

                    T3.join();

                    // After this, there were still overlapping trajectories, though they seemed less likely to occur. I assumed there's still an issue
                    // with synching the BITA parameters writing to the input file. So I tried to synchronize all three threads to basically run in
                    // concurrency. This didn't seem logical to me to have threads that I force to run one after the other, because it misses the whole
                    // idea of threading, letting them run in parallel. On the other hand though, no other solution seemed to work as I still got
                    // overlaps.


                    // If 3 way synchronizing works: I'm not sure synchronization overhead in this solution is preferred over a sleep() between threads. xxxx if sleeps even solves it.
                    *******************************************/
                    
                    /*
                    std::thread T2 = predictionSupplierCADAC->threadRunSupplierOnce(); // XXXX POSIX XXXX names xxxx no matter what, this will have to include process sych. cuz on the one hand displaying radar data and on the other hand running simulation and plotting it. i think it's a good first step to let it run on a seperate thread. xxxx mention: way of running once may vary.
                    predictionSupplierCADAC->trajectoryCADAC = std::make_shared<PredSuppTrajectoryCADAC>(currentCollectorLoadPaths.at(i), currentCollector->collectorKML_);
                    std::thread T3 = predictionSupplierCADAC->trajectoryCADAC->threadReadInput(false);                        
                    T2.join();
                    T3.join();
                    */
                    

                    /*
                    std::thread t1 = predictionSupplierCADAC->threadupdateBITA_ParamsInSupplierInput(trajectoryFromSensor.getBITA_Params());
                    t1.join();

                    std::thread t2 = predictionSupplierCADAC->threadRunSupplierOnce();
                    t2.join();

                    std::this_thread::sleep_for(std::chrono::milliseconds(30));

                    predictionSupplierCADAC->trajectoryCADAC = std::make_shared<PredSuppTrajectoryCADAC>(currentCollectorLoadPaths.at(i), currentCollector->collectorKML_);
                    std::thread t3 = predictionSupplierCADAC->trajectoryCADAC->threadReadInput(false);
                    //t1.join();
                    //t2.join();
                    t3.join();
                    */
                   
                    /* Not threaded*/
                    /* https://stackoverflow.com/questions/78799285/synchronize-three-threads-in-c-one-after-the-other/78801530#78801530 */
                   int first, second, third;

                    first = predictionSupplierCADAC->updateBITA_ParamsInSupplierInput(trajectoryFromSensor.getBITA_Params()); // XXXX names XXXX POSIX xxxx mention: way of updating bita params in input files may vary.
                    first ++;
                                                                                                                  // XXXX name XXXX POSIX                
                    second = predictionSupplierCADAC->runSupplierOnce(); // XXXX POSIX XXXX names xxxx no matter what, this will have to include process sych. cuz on the one hand displaying radar data and on the other hand running simulation and plotting it. i think it's a good first step to let it run on a seperate thread. xxxx mention: way of running once may vary.
                    second = first + 1;

                    predictionSupplierCADAC->trajectoryCADAC = std::make_shared<PredSuppTrajectoryCADAC>(currentCollectorLoadPaths.at(i), currentCollector->collectorKML_);
                    
                    third = second;
                    third = predictionSupplierCADAC->trajectoryCADAC->readInput(false);
                    third = second + 1;   
                    

                    //suppliersCollectorsVector.at(suppliersCollectorsVectorIndex)->suppliersVector.push_back(predictionSupplierCADAC->trajectoryCADAC); // XXXX
                    suppliersCollectorsVector.suppliersCollectorsVector_.at(suppliersCollectorsVector.getCurrentNumOfSuppliersCollectors())->suppliersVector.push_back(predictionSupplierCADAC->trajectoryCADAC);
                    //.at(suppliersCollectorsVectorIndex)->suppliersVector.push_back(predictionSupplierCADAC->trajectoryCADAC); // XXXX
                }
            }
            suppliersCollectorsVector.suppliersCollectorsVector_.at(suppliersCollectorsVector.getCurrentNumOfSuppliersCollectors())->currentNumOfSuppliers++;
            //suppliersCollectorsVector.at(suppliersCollectorsVectorIndex)->currentNumOfSuppliers++;
        }
        suppliersCollectorsVector.suppliersCollectorsVector_.at(suppliersCollectorsVector.getCurrentNumOfSuppliersCollectors())->plotCollectorAtOnce(2); // xxxx no "2" cases
        
        //suppliersCollectorsVector.at(suppliersCollectorsVectorIndex)->plotCollectorAtOnce(2); // xxxx parameterize xxxx try lock_guard with this and the two above in the loop to prevent from plotting 2 at the same time
        //suppliersCollectorsVectorIndex++; // XXXX leave in comment? do something else?
        suppliersCollectorsVector.currentNumOfSuppliersCollectors_++;

        //std::this_thread::sleep_for(std::chrono::milliseconds(1750)); // xxxx why is this needed..?
        std::this_thread::sleep_for(std::chrono::milliseconds(1450)); // xxxx why is this needed..?

        trajectoryFromSensor.setBITA_Params();
        
    } while (suppliersCollectorsVector.currentNumOfSuppliersCollectors_ <= 1);

    //globalcount = 0; // xxxx

    std::thread giveEstimation = decisionMaker.threadCalculate(syncSingleton);
    giveEstimation.join();

    pltThread.join(); // XXXX posix XXXX name XXXX explain why here, explain also for all pthreads.
    std::string command = "touch " + detectionKML;
    int systemReturn = std::system(command.c_str());
    












    std::this_thread::sleep_for(std::chrono::seconds(1)); // XXXX why is this here?


    
    //sleep(4);
    // printf("Signaling color\n");
    // pthread_cond_signal(&syncSingleton->condition_variable_color);
    // pthread_mutex_unlock(&syncSingleton->condition_lock_color);
    // printf("At this stage window should change color\n");
    
    //sleep(4);
    printf("Signaling \"finished\"\n");
    pthread_cond_signal(&syncSingleton->condition_variable_finished);
    pthread_mutex_unlock(&syncSingleton->condition_lock_finished);
    pthread_join(thread1, NULL);
    
   //pthread_join(thread1, NULL);

    //  XXXX Operate / do something when reaching a specific detect time value

    // while(std::stof(trajectoryFromSensor.getBITA_Params().tbal < 5)) {   // XXXX fix 5 XXXX fix names
    //     std::cout << "tbal: " << trajectoryFromSensor.getBITA_Params().tbal << std::endl;    // XXXX fix tbal XXXX maybe omit entirely the cout
    //     trajectoryFromSensor.setBITA_Params();
    // }

    // std::cout << "Reached detect time t = 5 [sec]" << std::endl;    // XXXX fix 5, fix english if needed.

    //return 0;

#endif

    // std::string command = "touch " + detectionKML;
	// int systemReturn = std::system(command.c_str());
    std::string command2 = "touch " + detectionKML;
    int systemReturn2 = std::system(command2.c_str());

    delete syncSingleton;
    return 0;
}