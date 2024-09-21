
#include "DecisionMaker.h"
#include <unistd.h>

int mutexheld = 0;

void DecisionMaker::calculate(SyncDataArrivalAndPredicting * syncSingleton) {


    //std::cout << "Setting alt data for collector 1 supplier 0" << std::endl;

    //std::unique_lock<std::mutex> ul(sensorTrajectory_->syncDetectSetBITA_mutex);
    //sensorTrajectory_->syncDetectSetBITA_cv.wait(ul, [&](){ return sensorTrajectory_->syncDetectSetBITA_ready; }); // xxxx here i learned in the tough way the imprtance of capturing by reference and not by value ENGLISH. put it?

    collectorsVector_->suppliersCollectorsVector_.at(1)->suppliersVector.at(0)->setAltData();

    //sensorTrajectory_->syncDetectSetBITA_ready = false;
    //ul.unlock();

    //sensorTrajectory_->syncDetectSetBITA_cv.notify_one();
    //ul.lock();


    std::vector<float> collector1supplier0alt = collectorsVector_->suppliersCollectorsVector_.at(1)->suppliersVector.at(0)->getAltData();

    int consequtiveConditionCount = 0;
    
    while(sensorTrajectory_->finishedPlotting == false) {

        // if (globalcount <= 5) // xxxx
        // {
        //     printf("Entered if (sensorTrajectory_->finishedplotting == false)\n");
        //     globalcount++;
        // }
        
        float tolerance = 50;
        /*
        estimation_ += 0.1;
        std::cout << "In calculate(), estimation = " << estimation_ << "\n" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // XXXX 
        */

        //if(collectorsVector_->suppliersCollectorsVector_.at(1)->suppliersVector.at(0)->SingleCoordsLine >= 50)


        if (sensorTrajectory_->finishedplotting2 == false)
        {
            printf("Entered if(sensorTrajectory_->finishedplotting2 == false)\n");

            std::unique_lock<std::mutex> ul(sensorTrajectory_->syncDetectSetBITA_mutex);

            

            std::cout << "Right before wait(). syncDetectSetBITA_ready: " << sensorTrajectory_->syncDetectSetBITA_ready << ". finishedPlotting:" << sensorTrajectory_->finishedPlotting << std::endl;
            //sleep(1);
            //mutexheld = sensorTrajectory_->syncDetectSetBITA_mutex.try_lock();
            //std::cout << "int pthread_mutex_trylock(pthread_mutex_t *mutex): " << mutexheld << std::endl;

            //sensorTrajectory_->syncDetectSetBITA_cv.wait(ul, [&](){ return sensorTrajectory_->syncDetectSetBITA_ready || sensorTrajectory_->finishedplotting2 == true; }); // xxxx here i learned in the tough way the imprtance of capturing by reference and not by value ENGLISH. put it?
            sensorTrajectory_->syncDetectSetBITA_cv.wait(ul, [&](){ return (sensorTrajectory_->syncDetectSetBITA_ready || sensorTrajectory_->finishedplotting2 == true); }); // xxxx here i learned in the tough way the imprtance of capturing by reference and not by value ENGLISH. put it?


            
            if(!utils::eq(collector1supplier0alt.size(), collector1supplier0alt, sensorTrajectory_->getCurrentAlt(), tolerance)) { consequtiveConditionCount++; }   // xxxx get current alt must be synchronized with currentrowindex...
            else consequtiveConditionCount = 0;    



            sensorTrajectory_->syncDetectSetBITA_ready = false;
            std::cout << "Turning syncDetectSetBITA_ready = false  (from DecisitonMaker.cpp)" << std::endl;
            ul.unlock();

            sensorTrajectory_->syncDetectSetBITA_cv.notify_one();
            ul.lock();

        }


        if (consequtiveConditionCount >= 50) {
            std::cout << "consequtiveConditionCount >= 5" << std::endl;
            check = true;
            printf("Signaling color\n");
            pthread_cond_signal(&syncSingleton->condition_variable_color);
            pthread_mutex_unlock(&syncSingleton->condition_lock_color);
            printf("At this stage window should change color\n");

        }    
            
    }
    
}

