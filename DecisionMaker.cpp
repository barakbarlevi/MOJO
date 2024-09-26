
#include "DecisionMaker.h"

int mutexheld = 0;

void DecisionMaker::calculate(SyncObject * syncObject) {

    collectorsVector_->at(1)->suppliersVector.at(0)->setAltData();

    std::vector<float> collector1supplier0alt = collectorsVector_->at(1)->suppliersVector.at(0)->getAltData();
    

    int consequtiveConditionCount = 0;
    
    while(sensorTrajectory_->getFinishedPlotting() == false) {

        
        float tolerance = 50;
        


        if (sensorTrajectory_->getFinishedPlotting2() == false)
        {
            printf("Entered if(sensorTrajectory_->finishedplotting2 == false)\n");

            //std::unique_lock<std::mutex> ul(sensorTrajectory_->syncDetectSetBITA_mutex);
            std::unique_lock<std::mutex> ul(syncObject->syncDetectSetBITA_mutex);

            

            //std::cout << "Right before wait(). syncDetectSetBITA_ready: " << sensorTrajectory_->syncDetectSetBITA_ready << ". finishedPlotting:" << sensorTrajectory_->finishedPlotting << std::endl; // xxxx necessary?
            std::cout << "Right before wait(). syncDetectSetBITA_ready: " << syncObject->syncDetectSetBITA_ready << ". finishedPlotting:" << sensorTrajectory_->getFinishedPlotting() << std::endl; // xxxx necessary?
            
            //sensorTrajectory_->syncDetectSetBITA_cv.wait(ul, [&](){ return (sensorTrajectory_->syncDetectSetBITA_ready || sensorTrajectory_->finishedplotting2 == true); }); // xxxx here i learned in the tough way the imprtance of capturing by reference and not by value ENGLISH. put it?
            syncObject->syncDetectSetBITA_cv.wait(ul, [&](){ return (syncObject->syncDetectSetBITA_ready || sensorTrajectory_->getFinishedPlotting2() == true); }); // xxxx here i learned in the tough way the imprtance of capturing by reference and not by value ENGLISH. put it?


            
            if(!utils::eq(collector1supplier0alt.size(), collector1supplier0alt, sensorTrajectory_->getCurrentAlt(), tolerance)) { consequtiveConditionCount++; }   // xxxx get current alt must be synchronized with currentrowindex...
            else consequtiveConditionCount = 0;    



            //sensorTrajectory_->syncDetectSetBITA_ready = false;
            syncObject->syncDetectSetBITA_ready = false;

            std::cout << "Turning syncDetectSetBITA_ready = false  (from DecisitonMaker.cpp)" << std::endl;
            ul.unlock();

            //sensorTrajectory_->syncDetectSetBITA_cv.notify_one();
            syncObject->syncDetectSetBITA_cv.notify_one();

            ul.lock();

        }


        if (consequtiveConditionCount >= 50) {
            std::cout << "consequtiveConditionCount >= 5" << std::endl;
            printf("Signaling color\n"); // xxxx this is happenning multile times. why? fix
            pthread_cond_signal(&syncObject->condition_variable_color);
            pthread_mutex_unlock(&syncObject->condition_lock_color);
            printf("At this stage window should change color\n");

        }    
            
    }
    
}

