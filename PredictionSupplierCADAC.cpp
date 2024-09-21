#include "PredictionSupplierCADAC.h"
#include "utils.h"

// XXXX FOR THE FOLLOWING (AND OTHERS?) EXPLAING ON CHAIN something... " : " in the comments.
PredictionSupplierCADAC::PredictionSupplierCADAC(std::string path_to_exe, std::string priamryInputFile) : PredictionSupplier(path_to_exe, priamryInputFile)
{
    /*

    XXXX OFCOURSE CHANGE ALL OF THE ABOVE TO SOMETHING RELEVANT. WRITE SOMETHING IN GENERAL FOR THE GENERAL USER. MENTION THAT AT THE MOEMNT, REFERRING SPECIFICALLY TO CADAC.

    In case of starting a PredictionSupplierCADAC instance, the following procedure expects:
    In paths_to_input_files:
        First string is "..\\tmp\\bal_name.tmp"
        Second string is path to TBM
    */
    this->path_to_exe = path_to_exe;           // xxxx add _ s for all class members for all classes ! starting here
    this->primaryInputFile = priamryInputFile; // xxxx names
}

int PredictionSupplierCADAC::prepareInputFiles() { return 0; }; // XXXX this is a big change, account for it. delete unnecessary code. explain what can be done here. init dt and dt_plot if u wanna change during flight. or any other input parameters THAT have no relation to BITA because bita editing should be included for any kind of simulation.
/*
int PredictionSupplierCADAC::prepareInputFiles() {
    // /-------------------------------------/
    // / First operation - edit bal_name.tmp /   // XXXX WRITE SOMETHING IN GENERAL FOR THE GENERAL USER. MENTION THAT AT THE MOEMNT, REFERRING SPECIFICALLY TO CADAC.
    std::ofstream fileBAL_NAME;
    std::string path_to_bal_name = paths_to_input_files[0]; // XXXX OFCOURSE THIS HAS TO CHANGE.
    fileBAL_NAME.open(path_to_bal_name, std::ios::out); // XXXX TODO: Switch to secure functions!!!    XXXX CHANGE NAMES.
    if(!fileBAL_NAME.is_open()) {   // XXXX AGAIN WITH THIS IF... CHECK IN JOHN's BASEMENT OR THE STEVENS INSTITUTE HOW TO PROPERLY WRITE THIS
        std::cerr << "Could not open file " << path_to_bal_name << std::endl;    // XXXX NAME HAS TO CHANGE. PRINT THAT EXITING BADLY. GO OVER ALL OF THESE CASES IN THE PROJECT.... WITH ONE GOOD way of dealing and stick with it
        return -1;
    }
    std::string line = this->primaryInputFile;  // XXXX DECLARATIONS UP...
    size_t pos = line.find_last_of('\\');   // XXXX OFCOURSE THIS HAS TO CHANGE TO CADAC...
    if (pos != std::string::npos) { pos++;  } // Point to character after the last '\\' XXXX EDIT THIS comment with cadac instead of nakar
    // XXXX ONE LINERS WHENEVER POSSIBLE.
    line = line.substr(pos, line.size() - pos);
    if(line.size() > 4) { line = line.substr(0, line.size() -4); }  // XXXX 4 is for nakar only? change if so
    else std::cerr << "Problem with the file name\n" << std::endl;  // XXXX And what doing with it?

    if (pos < line.size())  std::cout << "The first character after the last '\\' is: " << line[pos];    // XXXX probably need to change the \\. XXXX ONE LINERS WITHOUT {}
    else std::cout << "The string does not contain '\\'\n";

    fileBAL_NAME << line;   // XXXX this probably need to be chaned to match cadac.
    fileBAL_NAME.close();
    // /-------------------------------------/


    // /-------------------------------------/
    // /   Second operation - set dt_plot    /   // XXXX GIVE A GENERAL comment about how the user may need to decide number of operations needed for their simulation
    std::ifstream read_filePIF; // XXXX CHANGE THE NAMES !
    std::ofstream write_filePIF;

    read_filePIF.open(this->primaryInputFile, std::ios::out);   // TODO: Switch to secure functions! XXXX

    if(!read_filePIF.is_open()) { std::cerr << "Error openning file: " << this->primaryInputFile << std::endl; return -1;}

    write_filePIF.open("tempPREP.txt"); // XXXX THE HELL is this?
    if(!write_filePIF.is_open()) { std::cerr << "Error openning file: tempPREP.txt" << std::endl; return -1; } // XXXX the hell is this

    while(std::getline(read_filePIF, line)) {
        if(line.find("dtpl") != std::string::npos) {    // XXXX change ofcourse to fit cadac. XXXX this npos see if there's somehting more modern.
            std::cout << "Found 'dtpl' in the line\n";  // XXXX dtpl to something else, maybe omit some couts.
            pos = line.find_last_of(' ');    // XXXX comment what this is doing.
            if(pos != std::string::npos) {
                std::string last_word = line.substr(pos + 1);
                std::cout << "The last word in the string is: " << last_word << "\n";
                std::cout << "Now writing dt_plot, dt_plot = " << std::to_string(this->dt_plot) << "\n";
                std::cout << "Line before replaceLastWord: " << line << std::endl; // XXXX Consider omitting all these couts...
                utils::replaceLastWord(line, last_word, std::to_string(this->dt_plot));
                std::cout << "Line after replaceLastWord: " << line << std::endl;
                write_filePIF << line << std::endl;
            }
            else std::cout << "The string does not contain space";  // XXXX Check relevance.
        } else if (line.find("dtc") != std::string::npos) { // XXXX check relevance from nakar to cadac
            std::cout << "Found 'dtc' in the line\n";   // XXXX ofcourse change to relevant cadac
            pos = line.find_last_of(' ');    // XXXX comment what this is doing.
            if (pos != std::string::npos) { // XXXX what is this. must use npos? something else? what does npos mean?
                std::string last_word = line.substr(pos + 1);
                std::cout << "The last word in the string is: " << last_word << "\n";
                std::cout << "Now writing dt, dt = " << std::to_string(this->dt) << "\n";   // XXXX CHeck relevance of this couts...
                std::cout << "Line before replaceLastWord: " << line << std::endl;
                utils::replaceLastWord(line, last_word, std::to_string(this->dt));  // XXXX remember that bad GPT wrote replaceLastWord. TRY CHATGPT HERE ON INTERNET.
                std::cout << "Line after replaceLastWord: " << line << std::endl;
                write_filePIF << line << std::endl;
            } else std::cout << "The string does not contain space.\n";    // XXXX Looks like a very stupid print out. CTRL + F it and see what is this.

        } else write_filePIF << line << std::endl;
    }

    std::cout << "Right before closing the files\n";    // XXXX OMIT this one i did
    read_filePIF.close();
    write_filePIF.close();
    std::cout << "Right after closing the files\n"; // XXXX OMIT this one i did

    std::unique_lock<std::mutex> EditKML_ul(KML_editing_mutex);   // XXXX CHECK on name
    std::cout << "**********HERE**************\n"; // XXXX DELETE this line

    remove(this->primaryInputFile.c_str());
    rename("tempPREP.txt", this->primaryInputFile.c_str()); // XXXX

    EditKML_ul.unlock();

    return 0;
}
*/

// xxxx after unfruitfully ENGLISH trying to evade the temprary file by using seekp, https://stackoverflow.com/questions/732469/what-is-the-best-way-to-edit-the-middle-of-an-existing-flat-file made me "give up" ENGLISH and to it this way ENGLISH
// xxxx there might be a solution with mmap(), memmove() or MapViewOfFileEx() for windows but i'm not specnding time on this right now ENGLISH
// xxxx Assumptions: only changing the aerodynamics (ballistic coefficient in particular) between runs of CADAC ENGLISH xxxx
// xxxx Assumptions: fmass0  0\n EXPLAIN ON THIS HERE
// xxxx Assumptions: in this case, i took the entire 3 stage SLV and turned it into a single stage pure ballistic rocket. that means (ENGLISH) the input file is highly reduced (EnGLISH). all the _1 values are the same as the _0 values. the spi, fmass0 aren't 0 because the simulation doesn't handle it. but their values are pointless because fur flow rate is set to 0 which turns thrust to 0 and they have no meaning but they let the simulation run.
// xxxx refa refd matter? the -0.2 in moment whould override them? but they shold be meaningful for drag, so try lower trajs if seeing ABSOLUTELY no differences. the xcg_ref is for the moment so it makes sense that it doesnt change the results because i forces the moment. i set it to 0.01 just to emphasize that the value has no meaning but the simulation must get it as input which will be overriden.
// xxxx yes they matter. i saw it on gnuplot.
// xxxx xcg_0 and x_cg1 don't matter. saw it on gnuplot. moment = -0.2 must havr override them.
// xxxx not a kml

//void PredictionSupplierCADAC::updateBITA_ParamsInSupplierInput(BITA_params BITA_params) // XXXX so _bita_params or bita_params? and ofcourse change tbm and all
int PredictionSupplierCADAC::updateBITA_ParamsInSupplierInput(BITA_params BITA_params) // XXXX so _bita_params or bita_params? and ofcourse change tbm and all
{
    //this->trajectoryCADAC->synchObj.waitForCompletionOfT3();
    
    //BS1.acquire();
    //obj.waitForCompletionOfT3();
    //waitForCompletionOfT3();
    //std::unique_lock<std::mutex> ul(mux);         
    //cv.wait(ul, [&]() {return !completed[1]; }); 


    std::string inputPath = this->primaryInputFile;
    std::ofstream kml_file(inputPath, std::ofstream::out | std::ios::binary);

    if (!kml_file.is_open())
    {
        std::cout << std::strerror(errno) << '\n';
        std::cerr << "Failed to open the file: " << this->primaryInputFile << std::endl;
        //return -1;  // xxxx in other places these are some other things. decide and then fix 
    }
    std::string momentaryMass, aeroDeck;

    if (this->EmptyMasses.find(this->primaryInputFile) != this->EmptyMasses.end()) { momentaryMass = this->EmptyMasses[this->primaryInputFile]; } // xxxx explain on the temporary constraint to take only empty mass. probably written somewhere else. but mention here too, and why it's like this.
    else
        std::cout << "Key not found in EmptyMass unordered map.\n" << std::endl; // XXXX really not sure that when cheaping out on {}s like this, the if else structure works well.. CHECK !
    
    if (this->Aero_decks.find(this->primaryInputFile) != this->Aero_decks.end()) { aeroDeck = this->Aero_decks[this->primaryInputFile]; } // xxxx explain on the temporary constraint to take only empty mass. probably written somewhere else. but mention here too, and why it's like this.
    else
        std::cout << "Key not found in AeroDecks unordered map.\n" << std::endl; // XXXX really not sure that when cheaping out on {}s like this, the if else structure works well.. CHECK !

    kml_file << "TITLE input_ballistic.asc  Three-stage rocket ascent followed by ballistic\nMONTE 1 1234\nOPTIONS y_plot\nMODULES\nkinematics		def,init,exec\nenvironment		def,init,exec\npropulsion		def,init,exec\naerodynamics	def,init,exec\nforces			def,exec\nnewton			def,init,exec\neuler			def,init,exec\nintercept		def,exec\nEND\nTIMING\nscrn_step 10\nplot_step 0.5\ntraj_step 1\nint_step 0.003\ncom_step 20\nEND\nVEHICLES 1\nHYPER6 SLV\nlonx  " + BITA_params.lon + "\nlatx  " + BITA_params.lat + "\nalt  " + BITA_params.height + "\ndvbe  " + BITA_params.vbal + "\nphibdx  0\nthtbdx  " + BITA_params.gamalbal + "\npsibdx  " + BITA_params.azimlbal + "\nalpha0x  0\nbeta0x  0\n//environment\nmair  0\nWEATHER_DECK  /home/barak/Source_Files/CADAC/Custom/Version7/weather_deck_Wallops.asc\nRAYL dvae  5\ntwind  1\nturb_length  100\nturb_sigma  0.5\n//aerodynamics\nmaero  11\nAERO_DECK /home/barak/Source_Files/CADAC/Custom/Version7/" + aeroDeck + "\nxcg_ref  0.01\nrefa  3.243\nrefd  2.032\nalplimx  20\nalimitx  5\n//propulsion\nmprop  0\nvmass0  " + momentaryMass + "\nfmass0  0.01\nxcg_0  0.01\nxcg_1  0.01\nmoi_roll_0  6.95e3\nmoi_roll_1  6.95e3\nmoi_trans_0  158.83e3\nmoi_trans_1  158.83e3\nspi  0.01\nfuel_flow_rate  0.0\nEND\nENDTIME 900\nSTOP"; // xxxx check what to do with path containing my name
    /*
    kml_file << "TITLE input_ballistic.asc  Three-stage rocket ascent followed by ballistic\n \
                MONTE 1 1234\n \
                OPTIONS y_plot\n \
                MODULES\n \
                kinematics		def,init,exec\n \
                environment		def,init,exec\n \
                propulsion		def,init,exec\n \
                aerodynamics	def,init,exec\n \
                forces			def,exec\n \
                newton			def,init,exec\n \
                euler			def,init,exec\n \
                intercept		def,exec\n \
                END\n \
                TIMING\n \
                scrn_step 10\n \
                plot_step 0.5\n \
                traj_step 1\n \
                int_step 0.003\n \
                com_step 20\n \
                END\n \
                VEHICLES 1\n \
                HYPER6 SLV\n \
                lonx  " + BITA_params.lon + "\n \
                latx  " + BITA_params.lat + "\n \
                alt  " + BITA_params.height + "\n \
                dvbe  " + BITA_params.vbal + "\n \
                phibdx  0\n \
                thtbdx  " + BITA_params.gamalbal + "\n \
                psibdx  " + BITA_params.azimlbal + "\n \
                alpha0x  0\n \
                beta0x  0\n \
                //environment\n \
                mair  0\n \
                WEATHER_DECK  weather_deck_Wallops.asc\n \
                RAYL dvae  5\n \
                twind  1\n \
                turb_length  100\n \
                turb_sigma  0.5\n \
                //aerodynamics\n \
                maero  13\n \
                AERO_DECK " + aeroDeck + "\n \
                xcg_ref  8.6435\n \
                refa  3.243\n \
                refd  2.032\n \
                alplimx  20\n \
                alimitx  5\n \
                //propulsion\n \
                mprop  3\n \
                vmass0  " + momentaryMass + "\n \
                fmass0  0\n \
                xcg_0  7.02\n \
                xcg_1  6.98\n \
                moi_roll_0  21.94e3\n \
                moi_roll_1  6.95e3\n \
                moi_trans_0  671.62e3\n \
                moi_trans_1  158.83e3\n \
                spi  279.2\n \
                fuel_flow_rate  514.1\n \
                END\n \
                ENDTIME 900\n \
                STOP";
    */

    kml_file.close();
    std::cout << "Wrote to input file: lonx = " + BITA_params.lon + ", latx = " + BITA_params.lat + ", alt = " + BITA_params.height + ", dvbe = " + BITA_params.vbal + ", thtbdx = " + BITA_params.gamalbal + ", psibdx = " + BITA_params.azimlbal + ", Aerodeck = " + aeroDeck + ", vmass0 = " + momentaryMass << std::endl;
    
    //BS2.release();
    //obj.signalCompletionOfT1();
    //signalCompletionOfT1();
    //std::lock_guard<std::mutex> ul(mux);
    //completed[0] = true;
    //cv.notify_one();
    
    //return 1;
}

/*
int PredictionSupplierCADAC::updateBITA_ParamsInSupplierInput(BITA_params BITA_params) {    // XXXX so _bita_params or bita_params? and ofcourse change tbm and all
    std::ifstream read_fileUBS; // XXXX change name. XXXX see if can replace all this reading and writing streams with something better...
    std::ofstream write_fileUBS;    // XXXX change name. XXXX see if can replace all this reading and writing streams with something better...
    int BITA_ROWS_NUM = 12;
    int row = 0;

    read_fileUBS.open(this->primaryInputFile, std::ios::in | std::ios::out | std::ios::binary);    // XXXX ctrl + find on TBM. also see if all ios::s are necessary. XXXX comment: open file for *. XXXX see if there's a better way to do this. XXXX change name
    if (!read_fileUBS.is_open()) { std::cerr << "Error openning file: " << this->primaryInputFile << std::endl; return -1;} // XXXX after deciding on error showing convention, do a ctrl+f on "cerr" or others to change to whatever the convention is.

    write_fileUBS.open("tempBITA.txt"); // XXXX ofcourse change
    if (!write_fileUBS.is_open()) { std::cerr << "Error openning file: tempBITA.txt" << std::endl; return -1;} // XXXX after deciding on error showing convention, do a ctrl+f on "cerr" or others to change to whatever the convention is.

    std::string line;   // XXXX move declarations up in all code
    std::vector<std::string> BITA_params_vector = {BITA_params.tbal, BITA_params.massbal, BITA_params.sxbal, BITA_params.sybal, BITA_params.height, BITA_params.vbal, BITA_params.gamalbal, BITA_params.azimlbal, BITA_params.dvbal, BITA_params.etabal, BITA_params.phibal}; // XXXX of course change from nakar to cadac

    if(read_fileUBS.is_open()) {    // XXXX didnt we check that already....
        while(std::getline(read_fileUBS,line)) {
            line = line.substr(0, line.size() - 1); // XXXX not sure is needed here, we'll do debug and see..
            if (line == "BEGIN IN THE AIR") {   // XXXX ofcourse change from nakar to cadac
                std::cout << "Found \"Begin in the air\"" << std::endl;    // XXXX change
                write_fileUBS << line << std::endl;

                std::getline(read_fileUBS, line);   // This one is to skip (for now (? XXXX ) )the ibal = 2 line. Make sure it's 1. TODO: automade better. XXXX check relevance of all this.
                line = line.substr(0, line.size() - 1 );    // XXXX Dont like this .substr() and c_str and all. MAKE MODERN. is this plain c ? ctrl+ f .substr and .c_str and all.
                write_fileUBS << line << std::endl;

                for (int i = 0; i < BITA_ROWS_NUM -1; i++) {
                    std::getline(read_fileUBS, line);
                    line = line.substr(0, line.size() - 1 );
                    size_t last_space = line.find_last_of(' ');  // XXXX ctrl+f .find_last_of('') and change to proper name. change to last_space_poisition or something
                    if (last_space != std::string::npos) {
                        std::string last_word = line.substr(last_space + 1 );
                        std::cout << "Last word in line: " << last_word << "\n";

                        if ( i == 1 ) {      // Index that corresponds to massbal, that couldn't be taken from the "sensor", in this case CADAC. XXXX rewrite if need

                            // Making a significat shortcut here. Creating an unordered map of CADAC (simulation) threats XXXX / pls with their empty mass to fetch. A fully automated program should interpert it from a configuration (TBM XXXX) file.
                            // As documented in the manual, NAKAR's XXXX and what about CADAC's? BITA has no staging. takes the last specified stage XXXX .that means it cannot simulate engined flight. because drag coeffs are different compared to non-engined flight, and the last stage will have to be non-engined XXXX .
                            // When, in the future, will take XXXX rocket BITA's, there will be an option to simulate engined flight (not trivial) and then will have to evaluate mass depending on height XXXX writein some sort of way.

                            if(this->EmptyMasses.find(this->primaryInputFile) != this->EmptyMasses.end()) { BITA_params_vector.at(row) = this->EmptyMasses[this->primaryInputFile]; }   // XXXX change
                            else std::cout << "Key no found in unordered map.\n" << std::endl;  // XXXX really not sure that when cheaping out on {}s like this, the if else structure works well.. CHECK !
                        }

                        if ( i == 2 | i == 3 | i == 4 ) BITA_params_vector.at(row) = std::to_string(stof(BITA_params_vector.at(row)) / 1000);  // XXXX stof not modern change to something modern. XXXX / 1000 maybe not relevant here in cadac. arrange
                        if ( i == 6 ) BITA_params_vector.at(row) = std::to_string(stof(BITA_params_vector.at(row)) * (180.0 / 3.14159265));  // XXXX stof not modern change to something modern. XXXX / 1000 maybe not relevant here in cadac. arrange. XXXX find pi constant and replace.. from a library. XXXX Index that corresponds to flight path angle Gamma, needs to be converted to degrees in the case of current "sensor" CADAC. TODO: Fix the dependency on sensor format ! XXXX
                        utils::replaceLastWord(line, last_word, BITA_params_vector.at(row));


                    }

                    write_fileUBS << line << std::endl;
                    row++;
                }
            } else { write_fileUBS << line << std::endl; }

        }
        read_fileUBS.close();   // XXXX again with the multiple streams..
        write_fileUBS.close();  // XXXX again with the multiple streams..
        std::unique_lock<std::mutex> EditKML_ul(KML_editing_mutex); // XXXX name editing
        remove(this->primaryInputFile.c_str()); // XXXX
        rename("tempBITA.txt", this->primaryInputFile.c_str()); // XXXX
        EditKML_ul.unlock();
    } else std::cout << "Unable to open file\n"; // XXXX

    return 0;
}
*/

// xxxx delete if not needed. did it like this because nakar sucks and doesn't receive command line args.
/*
int PredictionSupplierCADAC::runSupplierOnce()
{                        // XXXX names and conventions
    FILE *fileCADAC_EXE; // XXXX CHANGE FROM C TO CPP ! AND THIS IS NO EXE ON LINUX! XXXX
    char input[] = "\n";
    fileCADAC_EXE = popen(this->path_to_exe.c_str(), "w"); // XXXX this line is way off. not exe on linux. find differences in open functions and decide, and comment, which one to take here. are there differences besides the argument list? mention and justify. XXXX c to cpp.
    if (fileCADAC_EXE == NULL)
    {
        printf("Error openning exe file.\n");
        exit(1);
    } // XXXX c to cpp. nullptr. or something better from the purple modern c++ book i remember he said something that replaces nullptr or NULL dont remember.

    // Sending input to the exe XXXX CHANGE
    fputs(input, fileCADAC_EXE); // XXXX why the hell init input in \n ?
    fflush(fileCADAC_EXE);
    printf("Used fflush\n"); // XXXX
    pclose(fileCADAC_EXE);   // XXXX
    return 0;
}
*/

int PredictionSupplierCADAC::runSupplierOnce()
//void PredictionSupplierCADAC::runSupplierOnce()
{ 
    //while(!readOnce) {
        //std::unique_lock<std::mutex> ul(gSyncOutputReading_mutex);   // xxxx 
        
        //BS2.acquire();
        // obj.waitForCompletionOfT1();
        //waitForCompletionOfT1();
        //std::unique_lock<std::mutex> ul(mux);             
        //cv.wait(ul, [&]() {return !completed[2]; }); 


        std::string COMMAND = this->path_to_exe + " " + this->primaryInputFile;
        std::system(COMMAND.c_str());

        //BS3.release();
        // obj.signalCompletionOfT2();
        //signalCompletionOfT2();
        //std::lock_guard<std::mutex> ul(mux);
        //completed[0] = false;
        //completed[1] = true;
        //cv.notify_one();


        //gSyncOutputReading_ready = true;
        //ul.unlock();
        //gSyncOutputReading_cv.notify_one();
        //ul.lock();
        //gSyncOutputReading_cv.wait(ul, [](){ return gSyncOutputReading_ready == false; });
    //}

    //readOnce = 0;
    
    //return 0;
}
