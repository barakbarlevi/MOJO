/*=============================================================================
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
has gained height. Parameter 'heightFirstDetection' of type double is set for this
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

  
=============================================================================*/

// xxxx put big intro text in grammarly
// XXXX go though all public / protected / private sections and decide on the best declarations.
// xxxx "int" i ? ++i or i++ ? xxxx always size_t? maybe unsigned?
// xxxx what abouth factory design pattern? how can it be implemented in mojo?
// XXXX difference from typedef struct?
// xxxx add option to specify port number as input and also path to CADAC so no "home + ..." but just "pathToCADAC + ..." and print out command structue and throw out if given wrongly
// XXXX EVERY LINE OF CODE SAYING CADAC SHOULD BE SWITCHED TO SOMETHING GENERAL. or maybe explain that it's general until at some level u have to be percise on simulation. but as less as possible.
// xxxx add std::cout << "log(-1) failed: " << std::strerror(errno) << '\n'; EVERYWHERE

#include "PredictionSupplierCADAC.h"
#include "DecisionMaker.h"
#include "X11_window.h"


enum availableSuppliers {
    CADAC,
    // Add more prediction suppliers (simulations) here.
};


int main(int argc, char *argv[])
{ 
    (void)argc;
    (void)argv;
  
    char* homeENV = getenv("HOME");
    std::stringstream ss;
    ss.str(homeENV);
    std::string home = ss.str();
    
    // Create a window background thread. Joined at the end of the program, after ground impact.
    pthread_t windowThread;
    SyncDataArrivalAndPredicting *syncSingleton = new SyncDataArrivalAndPredicting();
    pthread_create( &windowThread, NULL, windowWork, (void*) syncSingleton);
    

    std::vector<availableSuppliers> predictionSuppliers = {CADAC, CADAC, CADAC};

    // xxxx after changing to pathToCadac , mention it can be the path to any simulation
    // xxxx add "this is where input files for a specific simulation are stored.. why primary, exes, output loadpaths etc. explain why this is actually a table, together with the suppliers: the next four vector type declarations are... this is basically a table of constant cols and changing rows, that can chagne throughout the scenario"
    std::vector<std::string> currentCollectorPriamryInputFiles = {home + "/Source_Files/CADAC/Custom/Version7/inputOriginal.asc",
                                                                  home + "/Source_Files/CADAC/Custom/Version7/input_Drag0p7.asc",
                                                                  home + "/Source_Files/CADAC/Custom/Version7/input_Drag1p3.asc"}; // Absolute / relative paths of detection plots/tracks/simulation data. xxxx absolute path looks like crap FIX change to relative path. xxxx should be a vector in order to allow predictors modification in flight xxxx english
    std::vector<std::string> currentCollectorExecutables = {home + "/Source_Files/CADAC/Custom/Version7/SIX_DOF",
                                                            home + "/Source_Files/CADAC/Custom/Version7/SIX_DOF",
                                                            home + "/Source_Files/CADAC/Custom/Version7/SIX_DOF"}; // Absolute / relative paths of detection plots/tracks/simulation data. xxxx absolute path looks like crap FIX change to relative path. xxxx should be a vector in order to allow predictors modification in flight xxxx english
    std::vector<std::string> currentCollectorLoadPaths = {home + "/Source_Files/CADAC/Custom/Version7/cadacOutputVDXVDX.asc",
                                                          home + "/Source_Files/CADAC/Custom/Version7/cadacOutputVDXVDX.asc",
                                                          home + "/Source_Files/CADAC/Custom/Version7/cadacOutputVDXVDX.asc"}; // Absolute / relative paths of detection plots/tracks/simulation data. xxxx absolute path looks like crap FIX change to relative path. xxxx should be a vector in order to allow predictors modification in flight xxxx english xxxx explain that this also is a vector for the same reasons
    

    // Initialize necessary .kml files. Only Primary_Controller.kml has to be dragged into Google Earth.
    utils::kmlInitPrimaryController();
    utils::kmlInitSecondaryController();

    // At this point, the detected trajectory object is declared. It has to be declared as the specfic bottom derived
    // class from the Trajectory <- SensorTrajectory <- SensorTrajectory* hierarchy, because we need to know how to parse
    // information in the output file, and the structure of the file may change. Thread receiveDataFromRealtime runs 
    // until the last detection is recevied (ground impact).
    std::string detectionKML = "DetectionRT.kml";
    SensorTrajectoryCADAC trajectoryFromSensor("RT", detectionKML);
    std::thread receiveDataFromRealtime = trajectoryFromSensor.threadReceiveDataFromRT(syncSingleton);

    
    // Blocks until the first target detetion (message) is recevied over network.
    syncSingleton->WaitForFirstMsg();

    // Once it's recevied, we no longer wait on it. Reminder: Primary_Controller.kml has to be opened inside Google Earth.
    std::cout << "Started plotting inspected trajectory\n" << std::endl;

        
    #if 1
        
        // A while loop will continuously iterate until a certain condition on the Begin In The Air (BITA) parameters, which are the momentary
        // sampled state vector elements of the detected target, is met. This condtion could be set by the user, and should only rely on
        // detectable parameters. In the example below, we iterate until a certain height at ascent is reached. State vector (position, velocity, etc)
        // is sampled in synchronization with incoming detections: receive message -> sample -> receive message -> sample ....

        float heightFirstDetection(15000); // [meters]
        
        while ((std::stof(trajectoryFromSensor._BITA_Params.BITA_height) < heightFirstDetection) && (trajectoryFromSensor.get_vVertical() <= 0))
        {
            std::unique_lock<std::mutex> ul(trajectoryFromSensor.syncDetectSetBITA_mutex);
            trajectoryFromSensor.syncDetectSetBITA_cv.wait(ul, [&](){ return trajectoryFromSensor.syncDetectSetBITA_ready; });
            
            // Do work.
            trajectoryFromSensor.setBITA_Params();
            std::cout << "height: " << trajectoryFromSensor._BITA_Params.BITA_height << std::endl;

            trajectoryFromSensor.syncDetectSetBITA_ready = false;
            ul.unlock();

            trajectoryFromSensor.syncDetectSetBITA_cv.notify_one();
            ul.lock();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::cout << "Reached " << heightFirstDetection << "[m], at currentDetectionIndex: " << trajectoryFromSensor.currentDetectionIndex << std::endl;
        float detectionTime = std::stof(trajectoryFromSensor._BITA_Params.BITA_time);
        trajectoryFromSensor.reachedHdetection = true;

    #endif


    // Checkpoint: at this stage the entire detected trajectory should be received and plotted, with no analysis preformed on it. xxxx
    //receiveDataFromRealtime.join();


    #if 1

        // Once we've met the condition on the former while(), we can start do any kind of analysis we want. The do-while loop below
        // 

        std::vector<std::shared_ptr<SuppliersCollector>> VECTOR;
        DecisionMaker decisionMaker(&VECTOR, &trajectoryFromSensor);

        do
        {
            std::shared_ptr<SuppliersCollector> currentCollector = std::make_shared<SuppliersCollector>(std::stof(trajectoryFromSensor._BITA_Params.BITA_time) - detectionTime);
            VECTOR.push_back(currentCollector);
            currentCollector->collectorKML_ = "Collector" + std::to_string(VECTOR.size()) + ".kml";

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


                    // int first, second, third;

                    //     first = predictionSupplierCADAC->updateBITA_ParamsInSupplierInput(trajectoryFromSensor.getBITA_Params()); // XXXX names XXXX POSIX xxxx mention: way of updating bita params in input files may vary.
                    //     first ++;
                    //                                                                                                 // XXXX name XXXX POSIX                
                    //     second = predictionSupplierCADAC->runSupplierOnce(); // XXXX POSIX XXXX names xxxx no matter what, this will have to include process sych. cuz on the one hand displaying radar data and on the other hand running simulation and plotting it. i think it's a good first step to let it run on a seperate thread. xxxx mention: way of running once may vary.
                    //     second = first + 1;

                    //     predictionSupplierCADAC->trajectoryCADAC = std::make_shared<PredSuppTrajectoryCADAC>(currentCollectorLoadPaths.at(i), currentCollector->collectorKML_);
                        
                    //     third = second;
                    //     third = predictionSupplierCADAC->trajectoryCADAC->readInput(false);
                    //     third = second + 1;   
                        

                        predictionSupplierCADAC->updateBITA_ParamsInSupplierInput(trajectoryFromSensor.getBITA_Params()); // XXXX names XXXX POSIX xxxx mention: way of updating bita params in input files may vary.
                        predictionSupplierCADAC->runSupplierOnce(); // XXXX POSIX XXXX names xxxx no matter what, this will have to include process sych. cuz on the one hand displaying radar data and on the other hand running simulation and plotting it. i think it's a good first step to let it run on a seperate thread. xxxx mention: way of running once may vary.
                        predictionSupplierCADAC->trajectoryCADAC = std::make_shared<PredSuppTrajectoryCADAC>(currentCollectorLoadPaths.at(i), currentCollector->collectorKML_);
                        predictionSupplierCADAC->trajectoryCADAC->readInput(false);

                        //suppliersCollectorsVector.suppliersCollectorsVector_.at(suppliersCollectorsVector.getCurrentNumOfSuppliersCollectors())->suppliersVector.push_back(predictionSupplierCADAC->trajectoryCADAC);
                        VECTOR.back()->suppliersVector.push_back(predictionSupplierCADAC->trajectoryCADAC);
                    }
                }

                //suppliersCollectorsVector.suppliersCollectorsVector_.at(suppliersCollectorsVector.getCurrentNumOfSuppliersCollectors())->currentNumOfSuppliers++;
                VECTOR.back()->currentNumOfSuppliers++;

            }

            //suppliersCollectorsVector.suppliersCollectorsVector_.at(suppliersCollectorsVector.getCurrentNumOfSuppliersCollectors())->plotCollectorAtOnce(2); // xxxx no "2" cases
            VECTOR.back()->plotCollectorAtOnce(2); // xxxx no "2" cases
            
            //suppliersCollectorsVector.currentNumOfSuppliersCollectors_++;
            //increase VECTOR's size by one. needed manually? probably done by <vector> xxxx

            //std::this_thread::sleep_for(std::chrono::milliseconds(1750)); // xxxx why is this needed..?
            std::this_thread::sleep_for(std::chrono::milliseconds(1450)); // xxxx why is this needed..?

            trajectoryFromSensor.setBITA_Params();
            
        } while (VECTOR.size() <= 1);
        //while (suppliersCollectorsVector.currentNumOfSuppliersCollectors_ <= 1);

        //globalcount = 0; // xxxx

        std::thread giveEstimation = decisionMaker.threadCalculate(syncSingleton);
        giveEstimation.join();

        receiveDataFromRealtime.join(); // XXXX posix XXXX name XXXX explain why here, explain also for all pthreads.
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
        pthread_join(windowThread, NULL);
        

        //  XXXX Operate / do something when reaching a specific detect time value

        // while(std::stof(trajectoryFromSensor.getBITA_Params().BITA_time < 5)) {   // XXXX fix 5 XXXX fix names
        //     std::cout << "BITA_time: " << trajectoryFromSensor.getBITA_Params().BITA_time << std::endl;    // XXXX fix BITA_time XXXX maybe omit entirely the cout
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