#include "utils.h"
#include <math.h>

// XXXX NumOfEncounterToExclude or NumOfEncountersToExclude ?
// XXXX Use Chat GPT to write well and not crap. WRITE MODERN. GO OVER ALL CODE AND MAKE PRETTY.
// XXXX EXTENSIVE documentation throught all the project. ALL AND EXTENSIVE. for example what is functionality? make an explanation like fishman did in COB. purpose, input, output, etc.

namespace utils
{
    std::string SubStringStartTillReaching(const std::string &str, char c, int NumOfEncounterToExclude, int functionality, int currentDetectionIndex,  std::string caller, bool callerIsSensor)
    {
        if(callerIsSensor) std::cout << "currentDetectionIndex: " << currentDetectionIndex << " (Caller: " << caller << ")" << std::endl;
        else std::cout << "currentRowIndex: " << currentDetectionIndex << " (Caller: " << caller << ")" << std::endl;

        std::vector<size_t> pos; // XXXX ChatGPT doesnt use this.
        for (size_t i = 0; i < str.size(); ++i)
        {
            if (str[i] == c)
            {
                pos.push_back(i);
                if (pos.size() == (size_t)NumOfEncounterToExclude)
                    break;
            }
        }

        if (pos.size() < (size_t)NumOfEncounterToExclude)
        {
            std::cerr << "Size of pos < NumOfEncounterToExclude\n" << "NumOfEncounterToExclude: " << NumOfEncounterToExclude << ", str: " << str << " (Caller: " << caller << "\n"; // XXXX THIS IS VERY CHEAP WAY OF DEALING WITH AN ERROR. WRITE CONDITIONS on input in the description. assert what is needed.
            return str;
        }
        else
        {
            if (functionality == 0)
                return str.substr(0, pos[NumOfEncounterToExclude - 1]);
            if (functionality == 1)
                return str.substr(pos[NumOfEncounterToExclude - 2] + 1, pos[NumOfEncounterToExclude - 1] - pos[NumOfEncounterToExclude - 2] - 1);
        }

        return "Didn't find any other string"; // XXXX WHAT DOES THIS EVEN MEAN? AND what if functinality isn't 0 or 1 ? write properly.
    }

    void replaceLastWord(std::string &line, const std::string &old_word, const std::string &new_word)
    {
        size_t pos = line.rfind(old_word);
        if (pos != std::string::npos)
        {
            line.replace(pos, old_word.size(), new_word);
        }
    }

    std::string replaceLastWord(const std::string& line, const std::string& newWord) {
    size_t lastSpacePos = line.find_last_of(" ");
    if (lastSpacePos == std::string::npos) {
            return newWord; // If there's no space, just return the new word
        } else {
            return line.substr(0, lastSpacePos + 1) + newWord;
        }
    }

    

    hsv rgb2hsv(rgb in)
    {
        hsv out;
        double min, max, delta;

        min = in.r < in.g ? in.r : in.g;
        min = min < in.b ? min : in.b;

        max = in.r > in.g ? in.r : in.g;
        max = max > in.b ? max : in.b;

        out.v = max; // v
        delta = max - min;
        if (delta < 0.00001)
        {
            out.s = 0;
            out.h = 0; // undefined, maybe nan?
            return out;
        }
        if (max > 0.0)
        {                          // NOTE: if Max is == 0, this divide would cause a crash
            out.s = (delta / max); // s
        }
        else
        {
            // if max is 0, then r = g = b = 0
            // s = 0, h is undefined
            out.s = 0.0;
            out.h = NAN; // its now undefined
            return out;
        }
        if (in.r >= max)                   // > is bogus, just keeps compilor happy
            out.h = (in.g - in.b) / delta; // between yellow & magenta
        else if (in.g >= max)
            out.h = 2.0 + (in.b - in.r) / delta; // between cyan & yellow
        else
            out.h = 4.0 + (in.r - in.g) / delta; // between magenta & cyan

        out.h *= 60.0; // degrees

        if (out.h < 0.0)
            out.h += 360.0;

        return out;
    }

    rgb hsv2rgb(hsv in)
    {
        double hh, p, q, t, ff;
        long i;
        rgb out;

        if (in.s <= 0.0)
        { // < is bogus, just shuts up warnings
            out.r = in.v;
            out.g = in.v;
            out.b = in.v;
            return out;
        }
        hh = in.h;
        if (hh >= 360.0)
            hh = 0.0;
        hh /= 60.0;
        i = (long)hh;
        ff = hh - i;
        p = in.v * (1.0 - in.s);
        q = in.v * (1.0 - (in.s * ff));
        t = in.v * (1.0 - (in.s * (1.0 - ff)));

        switch (i)
        {
        case 0:
            out.r = in.v;
            out.g = t;
            out.b = p;
            break;
        case 1:
            out.r = q;
            out.g = in.v;
            out.b = p;
            break;
        case 2:
            out.r = p;
            out.g = in.v;
            out.b = t;
            break;

        case 3:
            out.r = p;
            out.g = q;
            out.b = in.v;
            break;
        case 4:
            out.r = t;
            out.g = p;
            out.b = in.v;
            break;
        case 5:
        default:
            out.r = in.v;
            out.g = p;
            out.b = q;
            break;
        }
        return out;
    }

    int kmlInitPrimaryController()
    {
        std::ofstream kml_file;
        kml_file.open("Primary_Controller.kml", std::ios::out | std::ios::binary);

        if (!kml_file.is_open())
        {
            std::cerr << "Failed to open the file: Primary_Controller.kml" << std::endl;
            return 1;
        }

        kml_file << "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n";
        kml_file << "\t<Document>\n\n\n";
        kml_file << "\t\t<NetworkLinkControl>\n";
        //kml_file << "\t\t\t<minRefreshPeriod>0.5</minRefreshPeriod>         <!-- float -->\n";
        kml_file << "\t\t\t<minRefreshPeriod>0.01</minRefreshPeriod>         <!-- float -->\n";
        kml_file << "\t\t\t<maxSessionLength>-1</maxSessionLength>          <!-- float -->\n";
        kml_file << "\t\t\t<linkName>Primary_Controller</linkName>          <!-- string -->\n";
        kml_file << "\t\t</NetworkLinkControl>\n\n";
        kml_file << "\t\t<visibility>1</visibility>\n\n";
        kml_file << "\t\t<NetworkLink>\n";
        kml_file << "\t\t\t<name>Main_Network_Link</name>\n";
        kml_file << "\t\t\t<refreshVisibility>1</refreshVisibility>\n";
        kml_file << "\t\t\t<flyToView>0</flyToView>\n";
        kml_file << "\t\t\t<Link>\n";
        kml_file << "\t\t\t\t<href>Secondary_Controller.kml</href>\n";

        kml_file << "\t\t\t\t<refreshMode>onInterval</refreshMode>       <!-- refreshModeEnum: onChange, onInterval, or onExpire -->\n";
        //kml_file << "\t\t\t\t<refreshInterval>0</refreshInterval>        <!-- float -->\n"; // xxxx
        kml_file << "\t\t\t\t<refreshInterval>0.01</refreshInterval>        <!-- float -->\n"; // xxxx
        // kml_file << "\t\t\t\t<refreshMode>onChange</refreshMode>       <!-- refreshModeEnum: onChange, onInterval, or onExpire -->\n";

        kml_file << "\t\t\t\t<viewRefreshMode>never</viewRefreshMode>    <!-- viewRefreshModeEnum: never, onStop, onRequest, onRegion -->\n";
        kml_file << "\t\t\t</Link>\n";
        kml_file << "\t\t</NetworkLink>\n\n\n";

        kml_file << "\t</Document>\n"; // XXXX Mention that this is not even really needed because these lines are going to be run over anyway but for the sake of good will write it.
        kml_file << "</kml>\n";

        kml_file.close();

        return 0;
    } // XXXX Arrange order of functions to be the same as in the .h file. xxxx same for all other source files.

    int kmlInitSecondaryController()
    {
        std::ofstream kml_file;
        kml_file.open("Secondary_Controller.kml", std::ios::out | std::ios::binary);

        if (!kml_file.is_open())
        {
            std::cerr << "Failed to open the file: Secondary_Controller.kml" << std::endl;
            return -1;  // xxxx in some other places it will be return 1; change it. what about exit() is it too C and not c++ ? find the right way to deal with an error and implement for all.
        }

        kml_file << "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n";
        kml_file << "\t<Document>\n\n\n";
        kml_file << "\t\t<NetworkLinkControl>\n";
        //kml_file << "\t\t\t<minRefreshPeriod>0.5</minRefreshPeriod>         <!-- float -->\n";
        kml_file << "\t\t\t<minRefreshPeriod>0.1</minRefreshPeriod>         <!-- float -->\n";
        kml_file << "\t\t\t<maxSessionLength>-1</maxSessionLength>          <!-- float -->\n";
        kml_file << "\t\t\t<linkName>Secondary_Controller</linkName>        <!-- string -->\n";
        kml_file << "\t\t</NetworkLinkControl>\n";
        kml_file << "\t\t<visibility>1</visibility>\n\n\n";

        kml_file << "\t</Document>\n"; // XXXX Mention that this is not even really needed because these lines are going to be run over anyway but for the sake of good will write it.
        kml_file << "</kml>\n";

        kml_file.close();

        return 0;
    }

    // XXXX CHECK ALL utility functions and make modern.
    /*
    int kmlInitTrajectory(std::string kml_path, std::string name)
    {
        std::ofstream kml_file;
        kml_file.open(kml_path);
        if (!kml_file.is_open())
        {
            std::cerr << "Error openning file: " << kml_path << std::endl;
            return -1;
        }

        std::string line;
        std::string lineToInsert;

        kml_file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n";
        kml_file << "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.opengis.net/kml/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.opengis.net/kml/2.2\">\n\n";
        kml_file << "\t<Document>\n";
        kml_file << "\t\t <name>" + name + "</name>\n\n\n";

        kml_file << "\t\t <Style id=\"\sn_shaded_dot0\"\>\n"; // XXXX is this working properly?
        kml_file << "\t\t\t<IconStyle>\n";
        kml_file << "\t\t\t\t<color>ff00AAff</color>\n";
        kml_file << "\t\t\t\t<scale>0.4</scale>\n";
        kml_file << "\t\t\t\t<Icon>\n";
        kml_file << "\t\t\t\t\t<href>http://maps.google.com/mapfiles/kml/shapes/shaded_dot.png</href>\n";
        kml_file << "\t\t\t\t</Icon>\n";
        kml_file << "\t\t\t</IconStyle>\n";
        kml_file << "\t\t\t<ListStyle></ListStyle>\n"; // XXXX A MUST? CHECK WITHOUT.
        kml_file << "\t\t</Style>\n";

        kml_file << "\t\t<Folder>\n";
        kml_file << "\t\t\t<name>LLAs</name>\n";
        kml_file << "\t\t</Folder>\n";

        kml_file << "\t</Document>\n";
        kml_file << "</kml>\n";

        kml_file.close();
        std::cout << "Closed the file :" << kml_path << "\n";

        return 0;
    }
    */


    int kmlInit_href(std::string kml_path, std::string name, int CollectorSize)
    {
        std::ofstream kml_file;
        kml_file.open(kml_path);
        if (!kml_file.is_open())
        {
            std::cerr << "Error openning file: " << kml_path << std::endl;
            return -1;
        }

        std::string line;
        std::string lineToInsert;

        kml_file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n";
        kml_file << "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.opengis.net/kml/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.opengis.net/kml/2.2\">\n\n";
        kml_file << "\t<Document>\n";
        kml_file << "\t\t <name>" + name + "</name>\n\n\n";

        utils::rgb RGB;
        utils::hsv HSV;
        std::stringstream ss;

        for (int i = 1; i < CollectorSize + 1; i++)
        {
            ss.str(std::string());
            HSV.h = ((((double)i) / (double)CollectorSize)) * 360.0; // XXXX THERES ABSOLUTELY no way that so many casts are needed. MINIMIZE.
            HSV.s = 0.9;
            HSV.v = 1;
            RGB = utils::hsv2rgb(HSV);

            int decimalR = static_cast<int>(RGB.r * 255);
            int decimalG = static_cast<int>(RGB.g * 255);
            int decimalB = static_cast<int>(RGB.b * 255);
            int decimalA = (1 * 255);

            ss << std::hex << std::setfill('0') << std::setw(2) << decimalA << std::hex << std::setfill('0') << std::setw(2) << decimalG << std::hex << std::setfill('0') << std::setw(2) << decimalB << std::hex << std::setfill('0') << std::setw(2) << decimalR << std::endl;
            std::string color = ss.str();                         // XXXX ss.str() ? FROm what library this and c_str, to_string .substr() etc are from ?
            
            kml_file << "\t\t <Style id=\"sn_shaded_dot" + std::to_string(i) + "\">\n"; // XXXX is this working properly?
            kml_file << "\t\t\t<IconStyle>\n";
            kml_file << "\t\t\t\t<color>" + color.substr(0, color.size() - 1) + "</color>\n";
            kml_file << "\t\t\t\t<scale>0.4</scale>\n";
            kml_file << "\t\t\t\t<Icon>\n";
            kml_file << "\t\t\t\t\t<href>http://maps.google.com/mapfiles/kml/shapes/shaded_dot.png</href>\n";
            kml_file << "\t\t\t\t</Icon>\n";
            kml_file << "\t\t\t</IconStyle>\n";
            kml_file << "\t\t\t<ListStyle></ListStyle>\n"; // XXXX A MUST? CHECK WITHOUT.
            kml_file << "\t\t</Style>\n";

        } // xxxx int i ?

        kml_file << "\t\t<Folder>\n";
        kml_file << "\t\t\t<name>LLAs</name>\n";
        kml_file << "\t\t</Folder>\n";

        kml_file << "\t</Document>\n";
        kml_file << "</kml>\n";

        kml_file.close();
        std::cout << "Closed the file :" << kml_path << "\n";

        return 0;
    } // XXXX




    int kmlInit_href(std::string kml_path, std::string name, std::string color) {

        std::ofstream kml_file;
        kml_file.open(kml_path);
        if (!kml_file.is_open())
        {
            std::cerr << "Error openning file: " << kml_path << std::endl;
            return -1;
        }

        std::string line;
        std::string lineToInsert;

        kml_file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n";
        kml_file << "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.opengis.net/kml/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.opengis.net/kml/2.2\">\n\n";
        kml_file << "\t<Document>\n";
        kml_file << "\t\t <name>" + name + "</name>\n\n\n";

        kml_file << "\t\t <Style id=\"sn_shaded_dot0\">\n"; // XXXX is this working properly?
        kml_file << "\t\t\t<IconStyle>\n";
        kml_file << "\t\t\t\t<color>" + color + "</color>\n";   
        kml_file << "\t\t\t\t<scale>0.4</scale>\n";
        kml_file << "\t\t\t\t<Icon>\n";
        kml_file << "\t\t\t\t\t<href>http://maps.google.com/mapfiles/kml/shapes/shaded_dot.png</href>\n";
        kml_file << "\t\t\t\t</Icon>\n";
        kml_file << "\t\t\t</IconStyle>\n";
        kml_file << "\t\t\t<ListStyle></ListStyle>\n"; // XXXX A MUST? CHECK WITHOUT.
        kml_file << "\t\t</Style>\n";

        kml_file << "\t\t<Folder>\n";
        kml_file << "\t\t\t<name>LLAs</name>\n";
        kml_file << "\t\t</Folder>\n";

        kml_file << "\t</Document>\n";
        kml_file << "</kml>\n";

        kml_file.close();
        std::cout << "Closed the file :" << kml_path << "\n";

        return 0;
    }
    















    int kmlInsertOneNetworkLink(std::string kml_path, std::string href)
    {
        std::fstream kml_file;
        kml_file.open(kml_path, std::ios::in | std::ios::out | std::ios::binary);

        if (!kml_file.is_open())
        {
            std::cerr << "Failed to open the file: " << kml_path << std::endl;
            return 1;
        }

        kml_file.seekp(-20, kml_file.end); // XXXX change -5 to the right number. xxxx delete when needed

        kml_file << "\t\t<NetworkLink>\n";
        // kml_file << "\t\t\t<name>" + kml_path + "</name>\n";    // XXXX make sure this is relative path and working. then write in comment that this can be a relative path
        kml_file << "\t\t\t<name>" + href + "</name>\n"; // XXXX make sure this is relative path and working. then write in comment that this can be a relative path
        kml_file << "\t\t\t<refreshVisibility>1</refreshVisibility>\n";
        kml_file << "\t\t\t<flyToView>0</flyToView>\n";
        kml_file << "\t\t\t<Link>\n";
        kml_file << "\t\t\t\t<href>" + href + "</href>\n";

        kml_file << "\t\t\t\t<refreshMode>onInterval</refreshMode>       <!-- refreshModeEnum: onChange, onInterval, or onExpire -->\n";
        //kml_file << "\t\t\t\t<refreshInterval>0</refreshInterval>        <!-- float -->\n"; // xxxx how small can it get? is there a closed solution? 0.001 didn't work. 0.01 didn't work.
        kml_file << "\t\t\t\t<refreshInterval>0.01</refreshInterval>        <!-- float -->\n"; // xxxx how small can it get? is there a closed solution? 0.001 didn't work. 0.01 didn't work.
        // kml_file << "\t\t\t\t<refreshMode>onChange</refreshMode>       <!-- refreshModeEnum: onChange, onInterval, or onExpire -->\n";

        kml_file << "\t\t\t\t<viewRefreshMode>never</viewRefreshMode>    <!-- viewRefreshModeEnum: never, onStop, onRequest, onRegion -->\n";
        kml_file << "\t\t\t</Link>\n";
        kml_file << "\t\t</NetworkLink>\n\n\n";

        kml_file << "\t</Document>\n"; // XXXX this is done because we ran over the ones that were before XXXX ENGLISH xxxx WRITE it in all other places when editing kmls.
        kml_file << "</kml>\n";

        kml_file.close();

        return 0;
    } // XXXX names after thought. delete this comment when needed. use these names / conventions on all others.

    /*
    int kmlInsertOneStyle(std::string kml_path, std::string styleID, std::string color, std::string scale) // xxxx fix !!! xxxx name of kml_path change to kmlPath
    {

        std::fstream kml_file;
        kml_file.open(kml_path, std::ios::in | std::ios::out | std::ios::binary);

        if (!kml_file.is_open())
        {
            std::cerr << "Failed to open the file: " << kml_path << std::endl;
            return 1;
        }

        kml_file.seekp(257, kml_file.beg); // XXXX change -5 to the right number. xxxx delete when needed

        // for (size_t i = 0; i < 15; i++) { kml_file << "\n"; }   // xxxx size_t ?

        // kml_file.seekp(257, kml_file.beg); // XXXX change -5 to the right number. xxxx delete when needed

        kml_file << "\t\t <Style id=\"\sn_shaded_dot" + styleID + "\"\>\n"; // XXXX is this working properly?
        kml_file << "\t\t\t<IconStyle>\n";
        kml_file << "\t\t\t\t<color>" + color + "</color>\n";
        kml_file << "\t\t\t\t<scale>" + scale + "</scale>\n";
        kml_file << "\t\t\t\t<Icon>\n";
        kml_file << "\t\t\t\t\t<href>http://maps.google.com/mapfiles/kml/shapes/shaded_dot.png</href>\n";
        kml_file << "\t\t\t\t</Icon>\n";
        kml_file << "\t\t\t</IconStyle>\n";
        kml_file << "\t\t\t<ListStyle></ListStyle>\n"; // XXXX A MUST? CHECK WITHOUT.
        kml_file << "\t\t</Style>\n";

        kml_file << "\t\t<Folder>\n\n\n";

        kml_file << "\t</Document>\n"; // XXXX this is done because we ran over the ones that were before XXXX ENGLISH xxxx WRITE it in all other places when editing kmls.
        kml_file << "</kml>\n";

        kml_file.close();

        return 0;
    } // XXXX names, XXXX more?
    */
 
   


    int kmlAppendOneCoord(std::string kml_path, std::string SingleCoordsLine, std::string styleID)
    {
        std::fstream kml_file;
        kml_file.open(kml_path, std::ios::in | std::ios::out | std::ios::binary);

        if (!kml_file.is_open())
        {
            std::cerr << "Failed to open the file: " << kml_path << std::endl;
            return 1;
        }

        kml_file.seekp(-32, kml_file.end); // XXXX change -5 to the right number. xxxx delete when needed

        kml_file << "\t\t\t<Placemark>\n";
        kml_file << "\t\t\t<name></name>\n";
        kml_file << "\t\t\t\t<styleUrl>sn_shaded_dot" + styleID + "</styleUrl>\n";
        // kml_file << "\t\t\t\t<description>Time: 15:00</description>\n"; // XXXX UPDATE description
        kml_file << "\t\t\t\t<Point>\n";
        kml_file << "\t\t\t\t\t<altitudeMode>absolute</altitudeMode>\n"; // XXXX Whats is it? check online. DELETE IF REDUNDANT. CHECK WITH AND WITHOUT
        kml_file << "\t\t\t\t\t<coordinates>" + SingleCoordsLine + "</coordinates>\n";
        kml_file << "\t\t\t\t</Point>\n";
        kml_file << "\t\t\t</Placemark>\n";
        kml_file << "\t\t</Folder>\n";

        kml_file << "\t</Document>\n"; // XXXX this is done because we ran over the ones that were before XXXX ENGLISH xxxx WRITE it in all other places when editing kmls.
        kml_file << "</kml>\n";

        kml_file.close();

        return 0;
        ;
    } // XXXX Kinda looks like bad practive to have these arguments. see if can modify and write better.

    // xxxx when ready, delete this ! and the declaration as well !
    /*
    int AppendOneCoordInKML(std::string KML, std::string SingleCoordsLine, std::string styleID, std::string SensorOrSupplier)
    {
        std::ifstream read_fileAOCIKML; // XXXX CHANGE STUPID NAMES
        std::ofstream write_fileAOCIKML;
        char buffer[100]; // XXXX WHY 100? what's the purpose of this buffer?

        read_fileAOCIKML.open(KML, std::ios::in | std::ios::out | std::ios::binary); // Open the file for both reading and writing. XXXX really needs both? RECHECK WHAT THE BAD GPT DID. XXXX ios?
        if (!read_fileAOCIKML.is_open())
        {
            // std::cerr << "(errno) Error openning file " << KML << ". Error: " << strerror_s(buffer, sizeof(buffer), errno) << std::endl;    // XXXX what's going on in this line?
            // std::cerr << "(GetLastError) Error openning file: " << KML << ". Error: " << GetLastError() << std::endl;
            std::cerr << "(errno) Error openning file " << KML << ". Error: XXXX FIND ERROR COMMAND" << std::endl; // XXXX what's going on in this line?
            return -1;
        }

        if (SensorOrSupplier == "Sensor")
        {
            write_fileAOCIKML.open("temp_KML_Sensor.txt");
        } // XXXX WTF is hapenning here check to see for better implementation. All this creating new file and copying... not efficient...
        else if (SensorOrSupplier == "Supplier")
        {
            write_fileAOCIKML.open("temp_KML_Supplier.txt");
        }
        else
        {
            std::cout << "Not a sensor and not a supplier, returning -1\n"; // XXXX Why cout ? make consistent all throughout. XXXX why returning -1 and not stating "exiting with -SOME BAD STATUS-" find online
            return -1;
        }

        if (!write_fileAOCIKML.is_open())
        {
            std::cerr << "Error openning file: temp_KML.txt" << std::endl; //  XXXX this is straight up wrong, there's not temp_KML
            return -1;
        }

        std::string line; // XXXX MOVE UP DECLARATIONS
        std::string lineToInsert;

        if (read_fileAOCIKML.is_open())
        { // XXXX WHY THIS IF AGAIN. DO ONLY ONCe.
            while (std::getline(read_fileAOCIKML, line))
            {
                // line = line.substr(0, line.size() -1 );
                if (line.find("</Folder>") != std::string::npos)
                { //    XXXX Make clear on the ::npos that the bad GPT brought. maybe there's something better.
                    std::cout << "Found \"</Folder>\"" << std::endl;
                    write_fileAOCIKML << "\t\t\t<Placemark>\n";
                    write_fileAOCIKML << "\t\t\t<name></name>\n";
                    write_fileAOCIKML << "\t\t\t\t<styleUrl>sn_shaded_dot" + styleID + "</styleUrl>\n";
                    write_fileAOCIKML << "\t\t\t\t<description>Time: 15:00</description>\n"; // XXXX UPDATE description
                    write_fileAOCIKML << "\t\t\t\t<Point>\n";
                    write_fileAOCIKML << "\t\t\t\t\t<altitudeMode>absolute</altitudeMode>\n"; // XXXX Whats is it? check online. DELETE IF REDUNDANT. CHECK WITH AND WITHOUT
                    write_fileAOCIKML << "\t\t\t\t\t<coordinates>" + SingleCoordsLine + "</coordinates>\n";
                    write_fileAOCIKML << "\t\t\t\t</Point>\n";
                    write_fileAOCIKML << "\t\t\t</Placemark>\n";
                    write_fileAOCIKML << "\t\t</Folder>\n";
                }
                else
                {
                    write_fileAOCIKML << line << std::endl;
                } // XXXX IN GENERAL, BAD PRACTICE
            }

            read_fileAOCIKML.close();
            write_fileAOCIKML.close();
            std::cout << "Closed the read_file: " << KML << "\n"; // XXXX MAKE BETTER.. why only couting on the file..

            remove(KML.c_str());
            // rename("temp_KML.txt",KML.c_str());
            if (SensorOrSupplier == "Sensor")
            {
                rename("temp_KML_Sensor.txt", KML.c_str());
            } // XXXX Is there even a .txt on linux ? make this code linux. but windows compatible as much as possible / portable
            if (SensorOrSupplier == "Supplier")
            {
                rename("temp_KML_Supplier.txt", KML.c_str());
            } // XXXX Is there even a .txt on linux ? make this code linux. but windows compatible as much as possible / portable
        }

        else
        {
            std::cout << "Unable to open file: " << KML << "\n";
        } // XXXX AND EXIT WITH A BAD STATUS ?
        return 0;
    } // XXXX Kinda looks like bad practive to have these arguments. see if can modify and write better.
    */

}
