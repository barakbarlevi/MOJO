#include "Trajectory.h"
#include <string.h>
//#include <Windows.h>    // XXXX WHAT FOR ? PORT TO POSIX.
#include "utils.h"
#include <cstring>

Trajectory::Trajectory(std::string loadPath, std::string kmlPath) {
    this->loadPath = loadPath;  // xxxx switch to loadPath_ i.e. add _ all throughout.
    this->KML_path = kmlPath;
    //this->ind = ind;    // xxxx
    //this->pl = pl;  // xxxx
    //this->mo = mo;  // xxxx
    //this->currentRowIndex = FirstLineOfNumericData;   // xxxx delete if not used
}

int Trajectory::readInput(bool isDetection) { //  XXXX MAKE SURE ALL VAR NAMES in all declaration and uses are the same. not like in cobra where there someIndex and SomeIdx for example.
//void Trajectory::readInput(bool isDetection) { //  XXXX MAKE SURE ALL VAR NAMES in all declaration and uses are the same. not like in cobra where there someIndex and SomeIdx for example.
    
    
    if(!isDetection) {
        


            std::ifstream file(this->loadPath);
            std::string line;

            if ((file.is_open()))   // XXXX THIS WAS ALL GENERATED WITH BAD GPT. REPLACE WHERE NEEDED. TO MODERN AND POSIX.
            {
                while(std::getline(file, line)) {
                    data.push_back(line + ',');
                    SingleCoordsLine = "0 , 0, 0";  // IMPORTANT: KEEP? DELETE? MOVE ? supposedly, helped with GE's view moving. but redundant code wise. at the very least move to some other geographic location.   
                }
                std::cout << "Finished a reading job" << std::endl;
                file.close();
            }
            else { std::cerr << "Unable to open file " << this->loadPath << std::endl;}  // XXXX std::cerr ? or something else? cout? are there other options? comment what std::cerr in fact IS. be consistent and change throught all code.    


            // obj.signalCompletionOfT3();
            //signalCompletionOfT3();
            //std::lock_guard<std::mutex> ul(mux);
            //completed[0] = false;
            //completed[1] = false;
            //completed[2] = true;
            //cv.notify_one();

            //gSyncOutputReading_ready = false;
            //readOnce = 1;
            //ul.unlock();
            //ul.lock();
        //}

    }

    else {
        std::ifstream file(this->loadPath);
        std::string line;

        if (file.is_open())   // XXXX THIS WAS ALL GENERATED WITH BAD GPT. REPLACE WHERE NEEDED. TO MODERN AND POSIX.
        {
            while(std::getline(file, line)) {
                data.push_back(line + ',');
                SingleCoordsLine = "0 , 0, 0";  // IMPORTANT: KEEP? DELETE? MOVE ? supposedly, helped with GE's view moving. but redundant code wise. at the very least move to some other geographic location.   
            }
            file.close();
        }
        else { 
            std::cout << std::strerror(errno) << '\n';
            std::cerr << "Unable to open file " << this->loadPath << std::endl;}  // XXXX std::cerr ? or something else? cout? are there other options? comment what std::cerr in fact IS. be consistent and change throught all code.    
    }

    //return 0;
}

/*
int Trajectory::kmlAppendOneCoord(std::string KML, std::string styleID, std::string SensorOrSupplier) {
    std::ifstream read_fileAOCIKML;  // XXXX CHANGE STUPID NAMES
    std::ofstream write_fileAOCIKML;
    char buffer[100];   // XXXX WHY 100? what's the purpose of this buffer?

    read_fileAOCIKML.open(KML, std::ios::in | std::ios::out | std::ios::binary);    // Open the file for both reading and writing. XXXX really needs both? RECHECK WHAT THE BAD GPT DID. XXXX ios? 
    if(!read_fileAOCIKML.is_open()) {
        //std::cerr << "(errno) Error openning file " << KML << ". Error: " << strerror_s(buffer, sizeof(buffer), errno) << std::endl;    // XXXX what's going on in this line?
        //std::cerr << "(GetLastError) Error openning file: " << KML << ". Error: " << GetLastError() << std::endl;
        std::cerr << "(errno) Error openning file " << KML << ". Error: XXXX FIND ERROR COMMAND" << std::endl;    // XXXX what's going on in this line?
        return -1;
    }

    if(SensorOrSupplier == "Sensor") {write_fileAOCIKML.open("temp_KML_Sensor.txt");}   // XXXX WTF is hapenning here check to see for better implementation. All this creating new file and copying... not efficient...
    else if (SensorOrSupplier == "Supplier") { write_fileAOCIKML.open("temp_KML_Supplier.txt");}
    else {
        std::cout << "Not a sensor and not a supplier, returning -1\n"; // XXXX Why cout ? make consistent all throughout. XXXX why returning -1 and not stating "exiting with -SOME BAD STATUS-" find online
        return -1;
    }

    if(!write_fileAOCIKML.is_open()) {
        std::cerr << "Error openning file: temp_KML.txt" << std::endl;  //  XXXX this is straight up wrong, there's not temp_KML
        return -1;
    }

    std::string line;   // XXXX MOVE UP DECLARATIONS
    std::string lineToInsert;

    if(read_fileAOCIKML.is_open()) {    // XXXX WHY THIS IF AGAIN. DO ONLY ONCe.
        while(std::getline(read_fileAOCIKML, line)) {
            //line = line.substr(0, line.size() -1 );
            if(line.find("</Folder>") != std::string::npos) { //    XXXX Make clear on the ::npos that the bad GPT brought. maybe there's something better.
            std::cout << "Found \"</Folder>\"" << std::endl;
            write_fileAOCIKML << "\t\t\t<Placemark>\n";
            write_fileAOCIKML << "\t\t\t<name></name>\n";
            write_fileAOCIKML << "\t\t\t\t<styleUrl>sn_shaded_dot" + styleID + "</styleUrl>\n";
            write_fileAOCIKML << "\t\t\t\t<description>Time: 15:00</description>\n";    // XXXX UPDATE description
            write_fileAOCIKML << "\t\t\t\t<Point>\n";
            write_fileAOCIKML << "\t\t\t\t\t<altitudeMode>absolute</altitudeMode>\n";   // XXXX Whats is it? check online. DELETE IF REDUNDANT. CHECK WITH AND WITHOUT
            write_fileAOCIKML << "\t\t\t\t\t<coordinates>" + SingleCoordsLine + "</coordinates>\n";
            write_fileAOCIKML << "\t\t\t\t</Point>\n";
            write_fileAOCIKML << "\t\t\t</Placemark>\n";
            write_fileAOCIKML << "\t\t</Folder>\n";
            }
            else {write_fileAOCIKML << line << std::endl;}  // XXXX IN GENERAL, BAD PRACTICE
        }

        read_fileAOCIKML.close();
        write_fileAOCIKML.close();
        std::cout << "Closed the read_file: " << KML << "\n";   // XXXX MAKE BETTER.. why only couting on the file..

        remove(KML.c_str());
        //rename("temp_KML.txt",KML.c_str());
        if (SensorOrSupplier == "Sensor") { rename("temp_KML_Sensor.txt", KML.c_str());}    // XXXX Is there even a .txt on linux ? make this code linux. but windows compatible as much as possible / portable
        if (SensorOrSupplier == "Supplier") { rename("temp_KML_Supplier.txt", KML.c_str());}    // XXXX Is there even a .txt on linux ? make this code linux. but windows compatible as much as possible / portable
    }

    else {std::cout << "Unable to open file: " << KML << "\n";} // XXXX AND EXIT WITH A BAD STATUS ?
    return 0;
}
*/

/*
int Trajectory::kmlInsertOneStyle(std::string KML, std::string styleID, std::string color, std::string scale) {
    std::ifstream read_file;
    std::ofstream write_file;

    read_file.open(KML, std::ios::in | std::ios::out | std::ios::binary);   // Open file for both reading and writing. XXXX THIS IS THE BAD GPT COMMENT. CHANGE ALL IF POSSIBLE
    if(!read_file.is_open()) { std::cerr << "Error openning file " << KML << std::endl; return -1;} // XXXX SAME WITH THE EXIT STATUSES AND WARNINGS AND notifying user from before.. also change whatever is possible from before and after to 1-liner like here.

    write_file.open("temp_KML.txt");    // XXXX I THINK IVE CHANGED THIS AT WORK
    if(!write_file.is_open()) { std::cerr << "Error openning file: temp_KML.txt\n" << std::endl; return -1;} // XXXX SAME WITH THE EXIT STATUSES AND WARNINGS AND notifying user from before.. also change whatever is possible from before and after to 1-liner like here.

    std::string line;   // XXXX TO THE BEGINNING PLZ
    std::string lineToInsert;   // XXXX TO THE BEGINNING PLZ

    if(read_file.is_open()){    // XXXX WHY THE HELL THIS IF AGAIN..
        while(std::getline(read_file,line)) {
            //line = line.substr(0, line.size() - 1 );
            if(line.find("<Folder>") != std::string::npos)  {   // XXXX AGAIN CHECK WITH THE NPOS... IS THERE AN OTHER/BETTER WAY..
                std::cout << "Found \"<Folder>\"" << std::endl;
                write_file << "\t\t <Style id=\"\sn_shaded_dot" + styleID + "\"\>\n";   // XXXX is this working properly?
                write_file << "\t\t\t<IconStyle>\n";
                write_file << "\t\t\t\t<color>" + color + "</color>\n";
                write_file << "\t\t\t\t<scale>" + scale + "</scale>\n";
                write_file << "\t\t\t\t<Icon>\n";
                write_file << "\t\t\t\t\t<href>http://maps.google.com/mapfiles/kml/shapes/shaded_dot.png</href>\n";
                write_file << "\t\t\t\t</Icon>\n";
                write_file << "\t\t\t</IconStyle>\n";
                write_file << "\t\t\t<ListStyle></ListStyle>\n";    // XXXX A MUST? CHECK WITHOUT.
                write_file << "\t\t</Style>\n";
                write_file << "\t\t<Folder>\n";
            }
            else {write_file << line << std::endl;} // XXXX PROBABLY STUPID, FIX.
        }

        read_file.close();  // XXXX USING THIS BAD GPS METHOD IS PROBABLY STUPID. SEE FOR A FIX.
        write_file.close();
        std::cout << "Closed the read_file\n";  // XXXX THIS IS A BAD LINE FOR COUT. BE MORE OFFICIAL.

        remove(KML.c_str());
        rename("temp_KML.txt",KML.c_str()); // XXXX
    }
    else {std::cout << "Unable to open file\n";}    // XXXX What file.. why.. ?
    return 0;
}
*/

/*
void Trajectory::plotTrajectoryAtOnce(Trajectory* Trajectory, std::string KML, int indexJump, int currentNumbebrOfSuppliers, int CollectorSize, float StyleScale) {
        
        utils::kmlInsertOneNetworkLink("Secondary_Controller.kml",KML); // xxxx names!
        utils::kmlInitTrajectory(KML, KML); // XXXX SHOULD BE REMOVED FROM HERE 
        utils::kmlInsertEntireTrajectory(Trajectory, KML, indexJump, currentNumbebrOfSuppliers, CollectorSize, StyleScale, 0, 0);    //  XXXX names etc

    }    
*/

void Trajectory::appendTrajectoryToKML(int indexJump, int delayms) {
        
        utils::kmlInsertOneNetworkLink("Secondary_Controller.kml",this->KML_path); // xxxx names!
        
        this->currentRowIndex = this->FirstLineOfNumericData_; // XXXX raw pointers ? move to smart ? or is it ok when a function argument? i think that its possible to pass smart pointers as function args..? https://stackoverflow.com/questions/65035189/whether-to-pass-shared-pointer-or-raw-pointer-to-a-function . do i take ownership here?
      
        std::cout << "Inserting Coord-By-Coord CADAC Trajectory to KML: " << this->KML_path << " at index jumps of: " << indexJump << std::endl; // XXXX English. check that this cout is needed. fix what needs to be fixed. maybe remove that cout maybe not
        for (unsigned int i = this->currentRowIndex; i < this->data.size() / indexJump; i++) {

            // currentRowIndex = i * indexJump; // XXXX makes sense to put in comment, add comment about it.
            
            // let's make this the producer 1 xxxx
            std::unique_lock<std::mutex> ul(this->syncDetectSetBITA_mutex);   // xxxx inside the loop, initialized every time? try putting outside of the loop see what happens.

            this->setSingleCoordsLine();
            std::this_thread::sleep_for(std::chrono::milliseconds(delayms));  // xxxx Using sleep here but really not sure that's the right way... https://stackoverflow.com/questions/70697368/how-to-let-a-thread-wait-itself-out-without-using-sleep
            utils::kmlAppendOneCoord(this->KML_path, this->SingleCoordsLine, "0"); // XXXX HERE this->SingleCoordsLine and line above just SingleCoordsLine ?xxxx fix this method to the one i printed on one paper that only does appending
            this->currentRowIndex += indexJump; // XXXX fix name of indexJump (?)

            this->syncDetectSetBITA_ready = true; // xxxx what the hell is syncDetectSetBITA_ready, and reachedHdetection. WRITE on how and why this synchronization is performed.
            ul.unlock();
            this->syncDetectSetBITA_cv.notify_one();
            
            ul.lock();

            if (!reachedHdetection){
                this->syncDetectSetBITA_cv.wait(ul, [this](){ return this->syncDetectSetBITA_ready == false; });
            }

        }
        finishedPlotting = true;
}

void Trajectory::appendTrajectoryToKML(int indexJump, int currentSupplierNumber, int CollectorSize, bool isCollector) {
        
        (void)CollectorSize;    // xxxx had to pass two ints that have different meaning! can't overload like that cuz both int so had to put a third arg! english. consult someone about this maybe
        
        if(!isCollector) utils::kmlInsertOneNetworkLink("Secondary_Controller.kml",this->KML_path); // xxxx names!    
        // xxxx if it is a collector, the collector will do it for itself ENGLISH
        
        this->currentRowIndex = this->FirstLineOfNumericData_; // XXXX raw pointers ? move to smart ? or is it ok when a function argument? i think that its possible to pass smart pointers as function args..? https://stackoverflow.com/questions/65035189/whether-to-pass-shared-pointer-or-raw-pointer-to-a-function . do i take ownership here?
      
        //std::cout << "Inserting Coord-By-Coord CADAC Trajectory to KML: " << this->KML_path << " at index jumps of: " << indexJump << std::endl; // XXXX English. check that this cout is needed. fix what needs to be fixed. maybe remove that cout maybe not
        for (unsigned int i = this->currentRowIndex; i < this->data.size() / indexJump; i++) {
            // currentRowIndex = i * indexJump; // XXXX makes sense to put in comment, add comment about it.
            this->setSingleCoordsLine();
            if(isCollector) {
                utils::kmlAppendOneCoord(this->KML_path, this->SingleCoordsLine, std::to_string(currentSupplierNumber + 1)); // XXXX HERE this->SingleCoordsLine and line above just SingleCoordsLine ?xxxx fix this method to the one i printed on one paper that only does appending
            } else {
                utils::kmlAppendOneCoord(this->KML_path, this->SingleCoordsLine, "0");
            }
            
                   
            this->currentRowIndex += indexJump; // XXXX fix name of indexJump (?)
        }
        
    }    