
#include "DecisionMaker.h"

DecisionMaker::DecisionMaker(std::vector<std::shared_ptr<SuppliersCollector>>* suppliersCollectorsVector, SensorTrajectoryCADAC* sensorTrajectory) {
        collectorsVector_ = suppliersCollectorsVector;
        sensorTrajectory_ = sensorTrajectory;
        estimation_ = 0;
    }

void DecisionMaker::calculate(SyncObject * syncObject) {

    int consequtiveConditionCount = 0;
    float altTolerance = 50;
    float consequtiveConditionCountThreshold = 50;
    collectorsVector_->at(1)->getSuppliersVector().at(0)->setAltData();
    std::vector<float> collector1supplier0alt = collectorsVector_->at(1)->getSuppliersVector().at(0)->getAltData();
    
    //while(sensorTrajectory_->getFinishedPlotting() == false) {
    while(syncObject->FINISHED == false) {

        //if (sensorTrajectory_->getFinishedPlotting2() == false)
        //{
            //printf("Entered if(sensorTrajectory_->finishedplotting2 == false)\n");
            printf("Entered while(syncObject->FINISHED == false)\n");

            std::unique_lock<std::mutex> ul(syncObject->syncDetectSetBITA_mutex_);

            //std::cout << "Right before wait(). syncDetectSetBITA_ready_: " << syncObject->syncDetectSetBITA_ready_ << ". finishedPlotting:" << sensorTrajectory_->getFinishedPlotting() << std::endl; // xxxx necessary?
            std::cout << "Right before wait(). syncDetectSetBITA_ready_: " << syncObject->syncDetectSetBITA_ready_ << ". syncObject->FINISHED:" << syncObject->FINISHED << std::endl; // xxxx necessary?
            
            //syncObject->syncDetectSetBITA_cv_.wait(ul, [&](){ return (syncObject->syncDetectSetBITA_ready_ || sensorTrajectory_->getFinishedPlotting2() == true); }); // xxxx here i learned in the tough way the imprtance of capturing by reference and not by value ENGLISH. put it?
            syncObject->syncDetectSetBITA_cv_.wait(ul, [&](){ return (syncObject->syncDetectSetBITA_ready_); }); // xxxx here i learned in the tough way the imprtance of capturing by reference and not by value ENGLISH. put it?

            if(!utils::eq(collector1supplier0alt.size(), collector1supplier0alt, sensorTrajectory_->getCurrentAlt(), altTolerance)) { consequtiveConditionCount++; }   // xxxx get current alt must be synchronized with currentrowindex...
            else consequtiveConditionCount = 0;    

            syncObject->syncDetectSetBITA_ready_ = false;

            std::cout << "Turning syncDetectSetBITA_ready_ = false  (from DecisitonMaker.cpp)" << std::endl;
            ul.unlock();

            syncObject->syncDetectSetBITA_cv_.notify_one();

            ul.lock();

        //}

        // This function doesn't currently address the situation where what once was thought of as
        // an irregularity that justified a notification had sice been restored. 
        if (consequtiveConditionCount >= consequtiveConditionCountThreshold) {
            std::cout << "consequtiveConditionCount >= 5" << std::endl;
            printf("Signaling color\n");
            pthread_cond_signal(&syncObject->condition_variable_color_);
            pthread_mutex_unlock(&syncObject->condition_lock_color_);
            printf("At this stage window should change color\n");
        }        
    }

    std::cout << "syncObject->FINISHED:" << syncObject->FINISHED << std::endl;
    std::cout << "DecisionMaker::calculate finished" << std::endl;
}