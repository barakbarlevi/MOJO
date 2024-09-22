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

// xxxx delete this if not used
int PredictionSupplierCADAC::prepareInputFiles() { return 0; }; // XXXX this is a big change, account for it. delete unnecessary code. explain what can be done here. init dt and dt_plot if u wanna change during flight. or any other input parameters THAT have no relation to BITA because bita editing should be included for any kind of simulation.




//void PredictionSupplierCADAC::updateBITA_ParamsInSupplierInput(BITA_params BITA_params) // XXXX so _bita_params or bita_params? and ofcourse change tbm and all
int PredictionSupplierCADAC::updateBITA_ParamsInSupplierInput(BITA_params BITA_params) // XXXX so _bita_params or bita_params? and ofcourse change tbm and all
{
    
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

    kml_file << "TITLE input_ballistic.asc  Three-stage rocket ascent followed by ballistic\nMONTE 1 1234\nOPTIONS y_plot\nMODULES\nkinematics		def,init,exec\nenvironment		def,init,exec\npropulsion		def,init,exec\naerodynamics	def,init,exec\nforces			def,exec\nnewton			def,init,exec\neuler			def,init,exec\nintercept		def,exec\nEND\nTIMING\nscrn_step 10\nplot_step 0.5\ntraj_step 1\nint_step 0.003\ncom_step 20\nEND\nVEHICLES 1\nHYPER6 SLV\nlonx  " + BITA_params.BITA_lon + "\nlatx  " + BITA_params.BITA_lat + "\nalt  " + BITA_params.BITA_height + "\ndvbe  " + BITA_params.BITA_speed + "\nphibdx  0\nthtbdx  " + BITA_params.BITA_flightPath + "\npsibdx  " + BITA_params.BITA_heading + "\nalpha0x  0\nbeta0x  0\n//environment\nmair  0\nWEATHER_DECK  /home/barak/Source_Files/CADAC/Custom/Version7/weather_deck_Wallops.asc\nRAYL dvae  5\ntwind  1\nturb_length  100\nturb_sigma  0.5\n//aerodynamics\nmaero  11\nAERO_DECK /home/barak/Source_Files/CADAC/Custom/Version7/" + aeroDeck + "\nxcg_ref  0.01\nrefa  3.243\nrefd  2.032\nalplimx  20\nalimitx  5\n//propulsion\nmprop  0\nvmass0  " + momentaryMass + "\nfmass0  0.01\nxcg_0  0.01\nxcg_1  0.01\nmoi_roll_0  6.95e3\nmoi_roll_1  6.95e3\nmoi_trans_0  158.83e3\nmoi_trans_1  158.83e3\nspi  0.01\nfuel_flow_rate  0.0\nEND\nENDTIME 900\nSTOP"; // xxxx check what to do with path containing my name
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
                lonx  " + BITA_params.BITA_lon + "\n \
                latx  " + BITA_params.BITA_lat + "\n \
                alt  " + BITA_params.BITA_height + "\n \
                dvbe  " + BITA_params.BITA_speed + "\n \
                phibdx  0\n \
                thtbdx  " + BITA_params.BITA_flightPath + "\n \
                psibdx  " + BITA_params.BITA_heading + "\n \
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
    std::cout << "Wrote to input file: lonx = " + BITA_params.BITA_lon + ", latx = " + BITA_params.BITA_lat + ", alt = " + BITA_params.BITA_height + ", dvbe = " + BITA_params.BITA_speed + ", thtbdx = " + BITA_params.BITA_flightPath + ", psibdx = " + BITA_params.BITA_heading + ", Aerodeck = " + aeroDeck + ", vmass0 = " + momentaryMass << std::endl;
    
    
}



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
{ 
    


        std::string COMMAND = this->path_to_exe + " " + this->primaryInputFile;
        std::system(COMMAND.c_str());
    
}
