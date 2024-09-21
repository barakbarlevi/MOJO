#include "SensorTrajectoryCADAC.h"
#include "utils.h"

SensorTrajectoryCADAC::SensorTrajectoryCADAC(std::string loadPath, std::string kmlPath) : SensorTrajectory(loadPath, kmlPath) {
    this->FirstLineOfNumericData_ = 0;
    this->currentRowIndex = this->FirstLineOfNumericData_;
       
} // XXXX names, : chain, an so

/*
// xxxx for now i just added "RT" in the instatiation line in main
SensorTrajectoryCADAC::SensorTrajectoryCADAC(std::string kmlPath) : SensorTrajectory(loadPath, kmlPath) {
    this->FirstLineOfNumericData_ = 1;
    this->currentRowIndex = this->FirstLineOfNumericData_;
    loadPath = "RT"; // xxxx check that this actually moves along that chain
       
} // XXXX names, : chain, an so
*/

void SensorTrajectoryCADAC::setBITA_Params()
{ // XXXX names.. make high level..
    
    //std::cout << "currentRowIndex: " << currentRowIndex << std::endl;

    /*
    this->_BITA_Params.tbal = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 1, 0); // XXXX check on func arguments to match cadac / general(?)
    // massbal: Cannot be detected by the sensor. It's added in he supplier's Update BITA routine, according to the supplier's models.  // XXXX VERIFY for example still calling it massbal ? go over this line.
    this->_BITA_Params.massbal = "0";
    this->_BITA_Params.lat = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 5, 1); // XXXX check on func arguments to match cadac / general(?)
    this->_BITA_Params.lon = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 4, 1); // XXXX check on func arguments to match cadac / general(?)
    this->_BITA_Params.height = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 6, 1);  // XXXX check on func arguments to match cadac / general(?)
    this->_BITA_Params.vbal = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 7, 1);  // XXXX check on func arguments to match cadac / general(?)
    // xxxx write that assuming alpha = 0 because can't sense it !
    this->_BITA_Params.gamalbal = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 3, 1);    // XXXX name gamalbal, XXXX to_string more modern way?  
    this->_BITA_Params.azimlbal = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 2, 1); // XXXX check on func arguments to match cadac / general(?) XXXX name azimlbal change
    */


    this->_BITA_Params.tbal = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 1, 0, currentRowIndex, "SensorTrajectoryCADAC::setBITA_Params 1"); // XXXX check on func arguments to match cadac / general(?)
    // massbal: Cannot be detected by the sensor. It's added in he supplier's Update BITA routine, according to the supplier's models.  // XXXX VERIFY for example still calling it massbal ? go over this line.
    this->_BITA_Params.massbal = "0";
    this->_BITA_Params.lat = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 5, 1, currentRowIndex, "SensorTrajectoryCADAC::setBITA_Params 2"); // XXXX check on func arguments to match cadac / general(?)
    this->_BITA_Params.lon = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 4, 1, currentRowIndex, "SensorTrajectoryCADAC::setBITA_Params 3"); // XXXX check on func arguments to match cadac / general(?)
    this->_BITA_Params.height = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 6, 1, currentRowIndex, "SensorTrajectoryCADAC::setBITA_Params 4");  // XXXX check on func arguments to match cadac / general(?)
    this->_BITA_Params.vbal = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 7, 1, currentRowIndex, "SensorTrajectoryCADAC::setBITA_Params 5");  // XXXX check on func arguments to match cadac / general(?)
    // xxxx write that assuming alpha = 0 because can't sense it !
    this->_BITA_Params.gamalbal = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 3, 1, currentRowIndex, "SensorTrajectoryCADAC::setBITA_Params 6");    // XXXX name gamalbal, XXXX to_string more modern way?  
    this->_BITA_Params.azimlbal = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 2, 1, currentRowIndex, "SensorTrajectoryCADAC::setBITA_Params 7"); // XXXX check on func arguments to match cadac / general(?) XXXX name azimlbal change

}

/*
int SensorTrajectoryCADAC::PlotTrajectoryCoordByCoord(int indexJump)
{                                                      // XXXX indexJump change name ? something more indicative ?
    std::cout << "Starting to plot main trajectory\n"; // XXXX not sure that "Starting to plot main trajectory" is exactly what should be written, if at all.
    for (unsigned int i = 1; i < data.size() / indexJump; i++)
    { // XXXX unsigned int i... something else? decide and be consistent

        // currentRowIndex += indexJump; // XXXX mention that commented on purpose because it's too long for demonstration purposes. even thought that what should have been for constant dt_plot / dt
        currentRowIndex = i * indexJump;
        SingleCoordsLine = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 3, 0); // XXXX

        std::unique_lock<std::mutex> EditKML_ul(KML_editing_mutex); // XXXX explaing on all mutexes why they are here. what's their purpse. what are they protecting. then set names to them and change all throughout. XXXX move to pthreads.

        // this->kmlAppendOneCoord(this->KML_path, std::to_string(0), "Sensor"); // Making a little shortcut and passing 0 for now. Assuming this is the only inspected trajectory as of the moment. // XXXX go over the comment.
        utils::kmlAppendOneCoord(this->KML_path, this->SingleCoordsLine, std::to_string(0), "Sensor"); // Making a little shortcut and passing 0 for now. Assuming this is the only inspected trajectory as of the moment. // XXXX go over the comment. XXXX HERE this->SingleCoordsLine and line above just SingleCoordsLine ?

        std::unique_lock<std::mutex> RunKML_ul(RunKML_mutex); // XXXX explaing on all mutexes why they are here. what's their purpse. what are they protecting. then set names to them and change all throughout. XXXX move to pthreads.

        std::thread t = threadLoadKML_ToGE(this->KML_path); // XXXX change name of t to something indicative. move to pthreads.

        t.join(); // XXXX change name of t to something indicative. move to pthreads.

        RunKML_ul.unlock(); // XXXX explaing on all mutexes why they are here. what's their purpse. what are they protecting. then set names to them and change all throughout. XXXX move to pthreads.

        EditKML_ul.unlock(); // XXXX explaing on all mutexes why they are here. what's their purpse. what are they protecting. then set names to them and change all throughout. XXXX move to pthreads.
    }
    return 0;
}
*/

/*
int SensorTrajectoryCADAC::kmlInsertEntireTrajectory(std::string KML, int indexJump, int currentNumOfSuppliers, int CollectorSize, float StyleScale) { // XXXX names... edit..
    std::cout << "Inserting entire SensorTrajectoryCADAC to KML: " << KML << " at index jumps of: " << indexJump << std::endl;   // XXXX English. check that this cout is needed. fix what needs to be fixed. XXXX fix cout
    std::string line;
    utils::rgb RGB;
    utils::hsv HSV;
    std::stringstream ss;

    HSV.h = ((((double)currentNumOfSuppliers) / (double)CollectorSize)) * 360.0;    // XXXX THERES ABSOLUTELY no way that so many casts are needed. MINIMIZE.
    HSV.s = 0.9;
    HSV.v = 1;
    RGB = utils::hsv2rgb(HSV);

    int decimalR = static_cast<int>(RGB.r * 255);
    int decimalG = static_cast<int>(RGB.g * 255);
    int decimalB = static_cast<int>(RGB.b * 255);
    int decimalA = (1 * 255);

    ss << std::hex << std::setfill('0') << std::setw(2) << decimalA << std::hex <<std::setfill('0') << std::setw(2) << decimalG << std::hex << std::setfill('0') << std::setw(2) << decimalB << std::hex << std::setfill('0') << std::setw(2) << decimalR << std::endl;
    std::string color = ss.str();   // XXXX ss.str() ? FROm what library this and c_str, to_string .substr() etc are from ?

    std::unique_lock<std::mutex> EditKML_ul(KML_editing_mutex); // XXXX name XXXX explaing on all mutexes why they are here. what's their purpse. what are they protecting. then set names to them and change all throughout. XXXX move to pthreads.

    //this->kmlInsertOneStyle(KML, std::to_string(currentNumOfSuppliers), color.substr(0, color.size() - 1), std::to_string(StyleScale)); // XXXX if unnecessary, delete
    utils::kmlInsertOneStyle(KML, std::to_string(currentNumOfSuppliers), color.substr(0, color.size() - 1), std::to_string(StyleScale));  // XXXX names etc


    this->currentRowIndex = 1;  // XXXX Compensation for output variables title in CADAC's output file.

    for(unsigned int i = 1; i < this->data.size() / indexJump; i++) {   // XXXX unsigned int ? i++ so why had there ++i ? strange ++i
        // currentRowIndex = i * indexJump; // XXXX makes sense to put in comment, add comment about it.
        //line = data[currentRowIndex]; // XXXX CLEAN IF DON"T NEED
        //SingleCoordsLine = line.substr(78,6) + "," + line.substr(66,6) + "," + line.substr(18,2) + line.substr(21,3);   // XXXX change ofc
        std::string lon  = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 4, 1); // XXXX check on func arguments to match cadac / general(?)
        std::string lat  = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 5, 1); // XXXX check on func arguments to match cadac / general(?)
        std::string alt  = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 6, 1); // XXXX check on func arguments to match cadac / general(?)
        SingleCoordsLine = lon + "," + lat +"," + alt;  // XXXX THERE MUST be a much more efficient way... ask chatgpt or think... at the end, in enhancements

        //this->kmlAppendOneCoord(KML, std::to_string(currentNumOfSuppliers), "Supplier");
        utils::kmlAppendOneCoord(KML, this->SingleCoordsLine, std::to_string(currentNumOfSuppliers), "Supplier"); // XXXX here this->singlecoordsline and line about no this-> ?

        currentRowIndex += indexJump;   // XXXX fix name of indexJump (?)
    }

    EditKML_ul.unlock();    // XXXX challenge the fact that mutexes are really where they should be. all throughout the code. XXXX explaing on all mutexes why they are here. what's their purpse. what are they protecting. then set names to them and change all throughout. XXXX move to pthreads.

    return 0;
}
*/

void SensorTrajectoryCADAC::setSingleCoordsLine()
{
    /*
    std::string lon = utils::SubStringStartTillReaching(this->data[this->currentRowIndex], ',', 4, 1); // XXXX check on func arguments to match cadac / general(?)  XXXX here i put this-> and below not. do both work? why? which to choose?
    std::string lat = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 5, 1); // XXXX check on func arguments to match cadac / general(?)
    std::string alt = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 6, 1); // XXXX check on func arguments to match cadac / general(?)
    */

    std::string lon = utils::SubStringStartTillReaching(this->data[this->currentRowIndex], ',', 4, 1, currentRowIndex, "SensorTrajectoryCADAC::setSingleCoordsLine 1"); // XXXX check on func arguments to match cadac / general(?)  XXXX here i put this-> and below not. do both work? why? which to choose?
    std::string lat = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 5, 1, currentRowIndex, "SensorTrajectoryCADAC::setSingleCoordsLine 2"); // XXXX check on func arguments to match cadac / general(?)
    std::string alt = utils::SubStringStartTillReaching(data[currentRowIndex], ',', 6, 1, currentRowIndex, "SensorTrajectoryCADAC::setSingleCoordsLine 3"); // XXXX check on func arguments to match cadac / general(?)


    this->SingleCoordsLine = lon + "," + lat + "," + alt;   // XXXX THERE MUST be a much more efficient way... ask chatgpt or think... at the end, in enhancements

} // XXXX names etc

//int SensorTrajectoryCADAC::PlotTrajectoryAtOnce(std::string KML, int indexJump, int currentNumbebrOfSuppliers, int CollectorSize, float StyleScale) {};
// XXXX in order to fill an entire sensor and an entire supplier, write for instance CADAC_As_Sensor and CADAC_As_Supplier. mention it in the paper and github. create a template for both.


void SensorTrajectoryCADAC::plotDataFromRT(SyncDataArrivalAndPredicting* syncSingleton)
{
    utils::kmlInsertOneNetworkLink("Secondary_Controller.kml",this->KML_path); // xxxx names!
        
    this->currentRowIndex = this->FirstLineOfNumericData_; // XXXX raw pointers ? move to smart ? or is it ok when a function argument? i think that its possible to pass smart pointers as function args..? https://stackoverflow.com/questions/65035189/whether-to-pass-shared-pointer-or-raw-pointer-to-a-function . do i take ownership here?

    std::cout << "Initializing server" << std::endl;

    int sock;
	socklen_t length;
	struct sockaddr_in server;
	int msgsock;
	char buf[1024];
    std::string buf_string;
	int rval;
    bool finishedOneDetection = false;

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0); 		// Create a socket that runs use the INET address family. the IP version 4. For a stream.
    if (sock < 0) {
	 perror("error: opening stream socket");
	 exit(1);
    }
    /* Name socket using wildcards */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = 0;
    server.sin_port = htons(36961);

    // The server has to bind a name on to its socket so that we can know what its address and port are, to tell clients.
    if (bind(sock, (sockaddr*)&server, sizeof(server))) {
	 perror("error: binding stream socket");
	 exit(1);
    }

    /* Find out assigned port number and print it out */
    length = sizeof(server);

    // getsockname - If you have a socket, and you want to know the name it's bound onto it, like the address and the port, we can get it with the function. It will change the arguments, passed as pointers.
    if (getsockname(sock, (sockaddr*)&server, &length)) {
	 perror("error: getting socket name");
	 exit(1);
    }

    printf("Socket has port #%d\n", ntohs(server.sin_port)); // Convertion from big endian to little endian.

    /* Start accepting connections */
    listen(sock, 5);

    // Get first message xxxx english etc
    do {
        printf("Still waiting. Haven't received first message yet\n");
	    msgsock = accept(sock, 0, 0); // Will block, waiting for a client to do a connect to the address with port from above.
	    if (msgsock == -1)
	        perror("error: accept");
	    else do {
	        bzero(buf, sizeof(buf)); // John: aweful. they've zeroed out this entire buffer every time they read any data in. but! we may or may not get all the data in one single call to read like we sent in one single call to write on the other end. just like the other end may or may not write the entire message in one call. it might send it over in chuncks. read also may not read the whole message on the first call, it might take multiple calls in order to recieve all the bytes in the message. it definitely will, if the message contained more than 1024 bytes. if we didn't get it all on the first try, it will return a nonzero value representing how many bytes it did get, print it and loop. why zeroing the buffer? because if it turns out the buffer has garbage in it, and you read stuff into the buffer that doesn't have a NULL at the end of the buffer when the read call is done, when you print it out it might print garbage forever in the memory of the machine that might follow the end of the buffer. printf(%s, buf) may be an unbounded amount of data. it's due to the factthat read() and write() are not mandated to deliver or transfer as many bytes as you ask in one call. can always give u less. so even though the client sent a bunch of chars and a null char at the end, if read() got all of it in one call, the print statements could be counted on to print the entire content of the buffer and hitting that null. would happen in C++ too. it's a feature of read(). by setting the entire buffer to zero first, you're guaranteed that you'll have a null character at the end of whatever u did recieve. he says it's awful because if rval tell u how many bytes were recieved in this call to read (one line below), if you recieve 1023 instead of 1024, there will always be room for a null at the end of whatever you recieved and down at the printf(%s,buf) statement could have said buf[rval] = 0. putting 1 null byte here is more efficient than putting a thousand in there every time u go around this loop. 
            if ((rval = read(msgsock, buf, 1024)) < 0) // read: here's the fd. in this case it's a socket, that was returned by accept(). read from my endpoint of this streaming socket, put the data inside buf. will read up to 1024 bytes.
                perror("error: reading stream message");
            if (rval == 0) // got EOF
                printf("Ending connection\n");
            else
            {
                std::cout << "Received first proper message" << std::endl;
                printf("%s\n", buf);

                buf[strlen(buf) - 1] = 0;
                buf_string = buf;
                data.push_back(buf);
                this->setSingleCoordsLine();
                //this->currentRowIndex++; // XXXX fix name of indexJump (?)
                
                syncSingleton->FirstMsgArrived(); // xxxx the name. NAMES
            }
        
                
        } while (syncSingleton->firstMsgArrived == false);





        // After first message has been recieved. No need to continue checking for xxxx
        /*do {
            msgsock = accept(sock, 0, 0); // Will block, waiting for a client to do a connect to the address with port from above.
            if (msgsock == -1)
                perror("error: accept");
            else do { */
        do {
            bzero(buf, sizeof(buf)); // John: aweful. they've zeroed out this entire buffer every time they read any data in. but! we may or may not get all the data in one single call to read like we sent in one single call to write on the other end. just like the other end may or may not write the entire message in one call. it might send it over in chuncks. read also may not read the whole message on the first call, it might take multiple calls in order to recieve all the bytes in the message. it definitely will, if the message contained more than 1024 bytes. if we didn't get it all on the first try, it will return a nonzero value representing how many bytes it did get, print it and loop. why zeroing the buffer? because if it turns out the buffer has garbage in it, and you read stuff into the buffer that doesn't have a NULL at the end of the buffer when the read call is done, when you print it out it might print garbage forever in the memory of the machine that might follow the end of the buffer. printf(%s, buf) may be an unbounded amount of data. it's due to the factthat read() and write() are not mandated to deliver or transfer as many bytes as you ask in one call. can always give u less. so even though the client sent a bunch of chars and a null char at the end, if read() got all of it in one call, the print statements could be counted on to print the entire content of the buffer and hitting that null. would happen in C++ too. it's a feature of read(). by setting the entire buffer to zero first, you're guaranteed that you'll have a null character at the end of whatever u did recieve. he says it's awful because if rval tell u how many bytes were recieved in this call to read (one line below), if you recieve 1023 instead of 1024, there will always be room for a null at the end of whatever you recieved and down at the printf(%s,buf) statement could have said buf[rval] = 0. putting 1 null byte here is more efficient than putting a thousand in there every time u go around this loop. 
            if ((rval = read(msgsock, buf, 1024)) < 0) // read: here's the fd. in this case it's a socket, that was returned by accept(). read from my endpoint of this streaming socket, put the data inside buf. will read up to 1024 bytes.
                perror("error: reading stream message");
            if (rval == 0) // got EOF
                printf("Ending connection\n");
            else

                if(buf[0] == '\0') 
                {
                    //this->currentRowIndex--;
                    //std::cout << "Turning syncDetectSetBITA_ready = true" << std::endl;
                    //this->syncDetectSetBITA_ready = true;
                    //finishedOneDetection = true;
                    //printf("Assigning finishedOneDetection = true;\n");
                    //this->finishedplotting2 = true;
                    //std::cout << "Assigning this->finishedplotting2 = true;" << std::endl;
                    //rval=0;




                    std::unique_lock<std::mutex> ul(this->syncDetectSetBITA_mutex);   // xxxx inside the loop, initialized every time? try putting outside of the loop see what happens.

                    //buf[strlen(buf) - 1] = 0;
                    //buf_string = buf;
                    //data.push_back(buf);
                    
                    //this->setSingleCoordsLine();
                    //utils::kmlAppendOneCoord(this->KML_path, this->SingleCoordsLine, "0"); // XXXX HERE this->SingleCoordsLine and line above just SingleCoordsLine ?xxxx fix this method to the one i printed on one paper that only does appending
                    //this->currentRowIndex++; // XXXX fix name of indexJump (?)

                    this->currentRowIndex--;
                    finishedOneDetection = true;
                    printf("Assigning finishedOneDetection = true;\n");
                    this->finishedplotting2 = true;
                    std::cout << "Assigning this->finishedplotting2 = true;" << std::endl;
                    rval=0;
                    
                    std::string command = "touch " + this->KML_path;
	                int systemReturn = std::system(command.c_str());

                    this->syncDetectSetBITA_ready = true;
                    std::cout << "Turning syncDetectSetBITA_ready = true" << std::endl;
                    ul.unlock();
                    this->syncDetectSetBITA_cv.notify_one();
                    
                    ul.lock();

                    if (!reachedHdetection){
                        this->syncDetectSetBITA_cv.wait(ul, [this](){ return this->syncDetectSetBITA_ready == false; });
                    }




                    
                }
                else
                {
                                    
                    printf("rval is:%d\n", rval);
                    printf("%s\n", buf);
                    std::cout << "Getting RT data and inserting it to KML: " << this->KML_path << std::endl; // XXXX English. check that this cout is needed. fix what needs to be fixed. maybe remove that cout maybe not
                    //for (unsigned int i = this->currentRowIndex; i < this->data.size() / indexJump; i++) {

                    // currentRowIndex = i * indexJump; // XXXX makes sense to put in comment, add comment about it.
                    
                    
                    std::unique_lock<std::mutex> ul(this->syncDetectSetBITA_mutex);   // xxxx inside the loop, initialized every time? try putting outside of the loop see what happens.

                    buf[strlen(buf) - 1] = 0;
                    buf_string = buf;
                    data.push_back(buf);
                    
                    this->setSingleCoordsLine();
                    utils::kmlAppendOneCoord(this->KML_path, this->SingleCoordsLine, "0"); // XXXX HERE this->SingleCoordsLine and line above just SingleCoordsLine ?xxxx fix this method to the one i printed on one paper that only does appending
                    this->currentRowIndex++; // XXXX fix name of indexJump (?)

                    this->syncDetectSetBITA_ready = true;
                    std::cout << "Turning syncDetectSetBITA_ready = true" << std::endl;
                    ul.unlock();
                    this->syncDetectSetBITA_cv.notify_one();
                    
                    ul.lock();

                    if (!reachedHdetection){
                        this->syncDetectSetBITA_cv.wait(ul, [this](){ return this->syncDetectSetBITA_ready == false; });
                    }
                }
        } while (rval != 0); // If there was an error and rval was -1, it would still print and won't even break from the loop.. the code simply demostrates socket functionality. taken from BSD's document.

        this->syncDetectSetBITA_ready = true;
        std::cout << "Turning syncDetectSetBITA_ready = true from right before closing socket" << std::endl;
        close(msgsock);
        printf("Closed msgsock\n");
        this->currentRowIndex--;
        printf("Decremented currentRowIndex by 1\n");
        finishedPlotting = true;

    } while (!finishedOneDetection);


    //sleep(1);
     /*
      * Since this program has an infinite loop, the socket "sock" is
      * never explicitly closed.  However, all sockets will be closed
      * automatically when a process is killed or terminates normally.
      */

    

} // XXXX names etc