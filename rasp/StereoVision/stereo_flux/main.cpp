#include <my_functions.h>
#include <my_functions.cpp>


int main(int argc, char** argv) 
{

    char rep;
    bool skip_calib = false;

    cout<<"Skip la calib ? [y/n]"<<endl;
    cin >> rep;

    if (rep == 'y') skip_calib = true;
    else skip_calib = false;

    if (rep != 'y') snaps(skip_calib);

   //if (skip_calib == false)
    //{
 //--------------------------------
        Size boardSize;
        string imagelistfn;
        bool showRectified;
        
        cv::CommandLineParser parser(argc, argv, "{w|8|}{h|5|}{nr||}{help||}{@input|../data/stereo_calib.xml|}");
        if (parser.has("help"))
            return print_help_calib();
        showRectified = !parser.has("nr");
        imagelistfn = parser.get<string>("@input");
        //boardSize.width = parser.get<int>("w");
        //boardSize.height = parser.get<int>("h");
        boardSize.width =8;
        boardSize.height = 5;
        if (!parser.check())
        {
            parser.printErrors();
            return 1;
        }
        vector<string> imagelist;
        bool ok = readStringList(imagelistfn, imagelist);
        if(!ok || imagelist.empty())
        {
            cout << "can not open " << imagelistfn << " or the string list is empty" << endl;
            return print_help_calib();
        }

        StereoCalib(skip_calib, imagelist, boardSize,false, true, showRectified);
   // }
    //------------------------------

    stereo_match(argc, argv);
   
    return 0;
}
