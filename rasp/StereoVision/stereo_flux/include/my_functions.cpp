#include "my_functions.h"


//PIPELINES RASPBERRY PI

std::string gstreamer_pipeline_camLeft(int capture_width, int capture_height, int framerate, int display_width, int display_height) {
    return
            " v4l2src device=/dev/video1  ! video/x-raw, "
            " width=(int)" + std::to_string(capture_width) + ","
            " height=(int)" + std::to_string(capture_height) + ","
            " framerate=(fraction)" + std::to_string(framerate) +"/1 !"
            " videoconvert ! videoscale !"
            " video/x-raw,"
            " width=(int)" + std::to_string(display_width) + ","
            " height=(int)" + std::to_string(display_height) + " ! appsink";
}

std::string gstreamer_pipeline_camRight(int capture_width, int capture_height, int framerate, int display_width, int display_height) {
    return
            " v4l2src device=/dev/video0  ! video/x-raw, "
            " width=(int)" + std::to_string(capture_width) + ","
            " height=(int)" + std::to_string(capture_height) + ","
            " framerate=(fraction)" + std::to_string(framerate) +"/1 !"
            " videoconvert ! videoscale !"
            " video/x-raw,"
            " width=(int)" + std::to_string(display_width) + ","
            " height=(int)" + std::to_string(display_height) + " ! appsink";
}

/////////////////////////////////PRENDRE DES PHOTOS//////////////////////////////
String inttostr(int input)
    {
        stringstream ss;
        ss << input;
        return ss.str();
    }

//----------------------------------------------------------------
int snaps (bool skip)
{
        //pipeline parameters
        int capture_width = 640; //1280 ;
        int capture_height = 480; //720 ;
        int framerate =  24;
        int display_width = 640; //1280 ;
        int display_height = 480; //720 ;
        
        
        std::string fichier_xml;

        if (skip == true) 
        {
            fichier_xml = "stereo_cal_skipped.xml";
        }
        else fichier_xml = "stereo_calib.xml";


        // Capture the two webcams
        //reset frame average
        std::string pipeline_camLeft = gstreamer_pipeline_camLeft(capture_width, capture_height, framerate,
                                                  display_width, display_height);
        std::string pipeline_camRight = gstreamer_pipeline_camRight(capture_width, capture_height, framerate,
                                                  display_width, display_height);
                                                  
        //
        std::cout << "(CAMLeft) Using pipeline: \n\t" << pipeline_camLeft << "\n\n\n";
        std::cout << "(CAMRight) Using pipeline: \n\t" << pipeline_camRight << "\n\n\n";

        // Capture the two webcamfilename : ../../stereo_calib2/data/calib_image_gauche/left16.jpg
        
        VideoCapture capL(pipeline_camLeft, cv::CAP_GSTREAMER);
        VideoCapture capR(pipeline_camRight, cv::CAP_GSTREAMER);

        namedWindow("Right");
        namedWindow("Left");

        char key = 'c';
        int count = 1;
        FileStorage fs;
        string filename = "../data/"+fichier_xml; //"/home/stereo-2022/StereoVision/stereo_pics/data/"+fichier_xml;
        string r,l;
        string chemin_dossier_calib = "../data";
        string chemin_dossier_img = "../data/Image"; //a definir
        Mat frameR,frameL;



        fs.open(filename, FileStorage::WRITE);
        fs << "strings" << "[";     // text - string sequence
        
        cout<<"Appuyez sur c pour prendre des capture pour la calibration"<<endl;
        cout<<"Appuyez sur s pour prendre une capture à traiter"<<endl;
        cout<<"Appuyez sur q pour passer a la suite"<<endl;

        // loop
        while(key != 'q') {
            capR >> frameR;
            capL >> frameL;

            // Show the two webcams
            imshow("Right",frameR); 
            imshow("Left",frameL); 

            if (key == 'c' && skip == false) {
                // Save file in their location if 'c' is pressed
                //stringstream Lname, Rname;

                r = chemin_dossier_calib + "/calib_image_droite/right"+inttostr(count)+".jpg";//a def
                l = chemin_dossier_calib + "/calib_image_gauche/left"+inttostr(count)+".jpg";// a def

                imwrite(r, frameR);
               
                fs << l;
                fs << r;
                        
                cout<<"Saved Left image and Right image ("<<count<<") for calibration"<<endl;
                count++;  
                    
            }
            if (key == 's') {
            // Save file in their location if 's' is pressed

            r = chemin_dossier_img + "/imageD"+".jpg";
			l = chemin_dossier_img + "/imageG"+".jpg";
						  
			imwrite(r, frameR);
			imwrite(l, frameL);
            cout<<"Saved Left image and Right image ("<<count<<") for processing"<<endl;
		}

            
            key = char(waitKey(1));
        }
        fs << "]";
        capR.release();
        capL.release();

        return 0;
}
///////////////////////////////////STEREO CALIB///////////////////////////////////////////

static int print_help_calib()
{
    cout <<
            " Given a list of chessboard images, the number of corners (nx, ny)\n"
            " on the chessboards, and a flag: useCalibrated for \n"
            "   calibrated (0) or\n"
            "   uncalibrated \n"
            "     (1: use cvStereoCalibrate(), 2: compute fundamental\n"
            "         matrix separately) stereo. \n"
            " Calibrate the cameras and display the\n"
            " rectified results along with the computed disparity images.   \n" << endl;
    cout << "Usage:\n ./stereo_calib -w=<board_width default=9> -h=<board_height default=6> <image list XML/YML file default=../data/stereo_calib.xml>\n" << endl;
    return 0;
}

//-------------------------------------------------------
static void StereoCalib(bool skip, const vector<string>& imagelist, Size boardSize,bool displayCorners = true, bool useCalibrated=true, bool showRectified=true)
{
    string chemin_intrinsics = "../data/";
    string chemin_extrinsics = "../data/";

    if (skip) return;

    if( imagelist.size() % 2 != 0 )
    {
        cout << "Error: the image list contains odd (non-even) number of elements\n";
        return;
    }

    const int maxScale = 2;
    const float squareSize = 3.f;  // Set this to your actual square size
    // ARRAY AND VECTOR STORAGE:

    vector<vector<Point2f> > imagePoints[2];
    vector<vector<Point3f> > objectPoints;
    Size imageSize;

    int i, j, k, nimages = (int)imagelist.size()/2;

    imagePoints[0].resize(nimages);
    imagePoints[1].resize(nimages);
    vector<string> goodImageList;

    for( i = j = 0; i < nimages; i++ )
    {
        for( k = 0; k < 2; k++ )
        {
            const string& filename = imagelist[i*2+k];
            Mat img = imread(filename, 0);
            cout << "filename : "<< filename << endl;
            if(img.empty())
                break;
            if( imageSize == Size() )
                imageSize = img.size();
            else if( img.size() != imageSize )
            {
                cout << "The image " << filename << " has the size different from the first image size. Skipping the pair\n";
                break;
            }
            bool found = false;
            vector<Point2f>& corners = imagePoints[k][j];
            for( int scale = 1; scale <= maxScale; scale++ )
            {
                Mat timg;
                if( scale == 1 )
                    timg = img;
                else
                    resize(img, timg, Size(), scale, scale);
                found = findChessboardCorners(timg, boardSize, corners,
                    CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
                if( found )
                {
                    if( scale > 1 )
                    {
                        Mat cornersMat(corners);
                        cornersMat *= 1./scale;
                    }
                    break;
                }
            }
            if( displayCorners )
            {
                cout << filename << endl;
                Mat cimg, cimg1;
                cvtColor(img, cimg, COLOR_GRAY2BGR);
                drawChessboardCorners(cimg, boardSize, corners, found);
                double sf = 640./MAX(img.rows, img.cols);
                resize(cimg, cimg1, Size(), sf, sf);
                imshow("corners", cimg1);
                char c = (char)waitKey(500);
                if( c == 27 || c == 'q' || c == 'Q' ) //Allow ESC to quit
                    exit(-1);
            }
            else
                putchar('.');
            if( !found )
                break;
            cornerSubPix(img, corners, Size(11,11), Size(-1,-1),
                         TermCriteria(TermCriteria::COUNT+TermCriteria::EPS,
                                      30, 0.01));
        }
        if( k == 2 )
        {
            goodImageList.push_back(imagelist[i*2]);
            goodImageList.push_back(imagelist[i*2+1]);
            j++;
        }
    }
    cout << j << " pairs have been successfully detected.\n";
    nimages = j;
    if( nimages < 2 )
    {
        cout << "Error: too little pairs to run the calibration\n";
        return;
    }

    imagePoints[0].resize(nimages);
    imagePoints[1].resize(nimages);
    objectPoints.resize(nimages);

    for( i = 0; i < nimages; i++ )
    {
        for( j = 0; j < boardSize.height; j++ )
            for( k = 0; k < boardSize.width; k++ )
                objectPoints[i].push_back(Point3f(k*squareSize, j*squareSize, 0));
    }

    cout << "Running stereo calibration ...\n";

    Mat cameraMatrix[2], distCoeffs[2];
    cameraMatrix[0] = initCameraMatrix2D(objectPoints,imagePoints[0],imageSize,0);
    cameraMatrix[1] = initCameraMatrix2D(objectPoints,imagePoints[1],imageSize,0);
    Mat R, T, E, F;

    double rms = stereoCalibrate(objectPoints, imagePoints[0], imagePoints[1],
                    cameraMatrix[0], distCoeffs[0],
                    cameraMatrix[1], distCoeffs[1],
                    imageSize, R, T, E, F,
                    CALIB_FIX_ASPECT_RATIO +
                    CALIB_ZERO_TANGENT_DIST +
                    CALIB_USE_INTRINSIC_GUESS +
                    CALIB_SAME_FOCAL_LENGTH +
                    CALIB_RATIONAL_MODEL +
                    CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5,
                    TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 100, 1e-5) );
    cout << "done with RMS error=" << rms << endl;

// CALIBRATION QUALITY CHECK
// because the output fundamental matrix implicitly
// includes all the output information,
// we can check the quality of calibration using the
// epipolar geometry constraint: m2^t*F*m1=0
    double err = 0;
    int npoints = 0;
    vector<Vec3f> lines[2];
    for( i = 0; i < nimages; i++ )
    {
        int npt = (int)imagePoints[0][i].size();
        Mat imgpt[2];
        for( k = 0; k < 2; k++ )
        {
            imgpt[k] = Mat(imagePoints[k][i]);
            undistortPoints(imgpt[k], imgpt[k], cameraMatrix[k], distCoeffs[k], Mat(), cameraMatrix[k]);
            computeCorrespondEpilines(imgpt[k], k+1, F, lines[k]);
        }
        for( j = 0; j < npt; j++ )
        {
            double errij = fabs(imagePoints[0][i][j].x*lines[1][j][0] +
                                imagePoints[0][i][j].y*lines[1][j][1] + lines[1][j][2]) +
                           fabs(imagePoints[1][i][j].x*lines[0][j][0] +
                                imagePoints[1][i][j].y*lines[0][j][1] + lines[0][j][2]);
            err += errij;
        }
        npoints += npt;
    }
    cout << "average epipolar err = " <<  err/npoints << endl;

    // save intrinsic parameters
    FileStorage fs(chemin_intrinsics+"intrinsics.yml", FileStorage::WRITE);
    if( fs.isOpened() )
    {
        fs << "M1" << cameraMatrix[0] << "D1" << distCoeffs[0] << "M2" << cameraMatrix[1] << "D2" << distCoeffs[1];
        fs.release();
    }
    else
        cout << "Error: can not save the intrinsic parameters\n";

    Mat R1, R2, P1, P2, Q;
    Rect validRoi[2];

    stereoRectify(cameraMatrix[0], distCoeffs[0],
                  cameraMatrix[1], distCoeffs[1],
                  imageSize, R, T, R1, R2, P1, P2, Q,
                  CALIB_ZERO_DISPARITY, 1, imageSize, &validRoi[0], &validRoi[1]);

    fs.open(chemin_extrinsics+"extrinsics.yml", FileStorage::WRITE);
    if( fs.isOpened() )
    {
        fs << "R" << R << "T" << T << "R1" << R1 << "R2" << R2 << "P1" << P1 << "P2" << P2 << "Q" << Q;
        fs.release();
    }
    else
        cout << "Error: can not save the extrinsic parameters\n";

    // OpenCV can handle left-right
    // or up-down camera arrangements
    bool isVerticalStereo = fabs(P2.at<double>(1, 3)) > fabs(P2.at<double>(0, 3));

// COMPUTE AND DISPLAY RECTIFICATION
    if( !showRectified )
        return;

    Mat rmap[2][2];
// IF BY CALIBRATED (BOUGUET'S METHOD)
    if( useCalibrated )
    {
        // we already computed everything
    }
// OR ELSE HARTLEY'S METHOD
    else
 // use intrinsic parameters of each camera, but
 // compute the rectification transformation directly
 // from the fundamental matrix
    {
        vector<Point2f> allimgpt[2];
        for( k = 0; k < 2; k++ )
        {
            for( i = 0; i < nimages; i++ )
                std::copy(imagePoints[k][i].begin(), imagePoints[k][i].end(), back_inserter(allimgpt[k]));
        }
        F = findFundamentalMat(Mat(allimgpt[0]), Mat(allimgpt[1]), FM_8POINT, 0, 0);
        Mat H1, H2;
        stereoRectifyUncalibrated(Mat(allimgpt[0]), Mat(allimgpt[1]), F, imageSize, H1, H2, 3);

        R1 = cameraMatrix[0].inv()*H1*cameraMatrix[0];
        R2 = cameraMatrix[1].inv()*H2*cameraMatrix[1];
        P1 = cameraMatrix[0];
        P2 = cameraMatrix[1];
    }

    //Precompute maps for cv::remap()
    initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);

    Mat canvas;
    double sf;
    int w, h;
    if( !isVerticalStereo )
    {
        sf = 600./MAX(imageSize.width, imageSize.height);
        w = cvRound(imageSize.width*sf);
        h = cvRound(imageSize.height*sf);
        canvas.create(h, w*2, CV_8UC3);
    }
    else
    {
        sf = 300./MAX(imageSize.width, imageSize.height);
        w = cvRound(imageSize.width*sf);
        h = cvRound(imageSize.height*sf);
        canvas.create(h*2, w, CV_8UC3);
    }

    for( i = 0; i < nimages; i++ )
    {
        for( k = 0; k < 2; k++ )
        {
            //Mat img = imread("../data/"+goodImageList[i*2+k], 0), rimg, cimg;
            Mat img = imread(goodImageList[i*2+k], 0), rimg, cimg;
            remap(img, rimg, rmap[k][0], rmap[k][1], INTER_LINEAR, BORDER_CONSTANT,Scalar(0,0,0));
            cvtColor(rimg, cimg, COLOR_GRAY2BGR);
            Mat canvasPart = !isVerticalStereo ? canvas(Rect(w*k, 0, w, h)) : canvas(Rect(0, h*k, w, h));
            resize(cimg, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);
            if( useCalibrated )
            {
                Rect vroi(cvRound(validRoi[k].x*sf), cvRound(validRoi[k].y*sf),
                          cvRound(validRoi[k].width*sf), cvRound(validRoi[k].height*sf));
                rectangle(canvasPart, vroi, Scalar(0,0,255), 3, 8);
            }
        }

        if( !isVerticalStereo )
            for( j = 0; j < canvas.rows; j += 16 )
                line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1, 8);
        else
            for( j = 0; j < canvas.cols; j += 16 )
                line(canvas, Point(j, 0), Point(j, canvas.rows), Scalar(0, 255, 0), 1, 8);
        imshow("rectified", canvas);
        char c = (char)waitKey();
        if( c == 27 || c == 'q' || c == 'Q' )
            break;
    }
}

//-----------------------------------------------------------------------------------

static bool readStringList( const string& filename, vector<string>& l )
{
    l.resize(0);
    FileStorage fs(filename, FileStorage::READ);
    if( !fs.isOpened() )
        return false;
    FileNode n = fs.getFirstTopLevelNode();
    if( n.type() != FileNode::SEQ )
        return false;
    FileNodeIterator it = n.begin(), it_end = n.end();
    for( ; it != it_end; ++it )
        l.push_back((string)*it);
    return true;
}

//-----------------------------------------------------------------------------------------
/*int stereo_calib (int argc, char** argv)
{

    Size boardSize;
    string imagelistfn;
    bool showRectified;
    cv::CommandLineParser parser(argc, argv, "{w|9|}{h|6|}{nr||}{help||}{@input|../data/stereo_calib.xml|}");
   
    if (parser.has("help"))
        return print_help_calib();
    showRectified = !parser.has("nr");
    imagelistfn = parser.get<string>("@input");
    boardSize.width = parser.get<int>("w");
    boardSize.height = parser.get<int>("h");
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

    StereoCalib(imagelist, boardSize,false, true, showRectified);

    return 0;
}*/
////////////////////////////////STEREO MATCH////////////////////////////////////
static void print_help_stereo_match()
{
    printf("\nDemo stereo matching converting L and R images into disparity and point clouds\n");
    printf("\nUsage: stereo_match <left_image> <right_image> [--algorithm=bm|sgbm|hh] [--blocksize=<block_size>]\n"
           "[--max-disparity=<max_disparity>] [--scale=scale_factor>] [-i <intrinsic_filename>] [-e <extrinsic_filename>]\n"
           "[--no-display] [-o <disparity_image>] [-p <point_cloud_file>]\n");
}

//---------------------------------------------------------------------------------
static void saveXYZ(const char* filename, const Mat& mat)
{
    const double max_z = 1.0e4;
    FILE* fp = fopen(filename, "wt");
    for(int y = 0; y < mat.rows; y++)
    {
        for(int x = 0; x < mat.cols; x++)
        {
            Vec3f point = mat.at<Vec3f>(y, x);
            if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) continue;
            fprintf(fp, "%f %f %f\n", point[0], point[1], point[2]);
        }
    }
    fclose(fp);
}
//-----------------------------------------------------------------------------------
int stereo_match(int argc, char** argv)
{
    const char* algorithm_opt = "--algorithm=";
    const char* maxdisp_opt = "--max-disparity=";
    const char* blocksize_opt = "--blocksize=";
    const char* nodisplay_opt = "--no-display";
    const char* scale_opt = "--scale=";
    Mat R, T, R1, P1, R2, P2;
    Mat M1, D1, M2, D2;
    Mat map11, map12, map21, map22;
    Mat img1r, img2r;
    Mat disp, disp8;
    int cn;
    int sgbmWinSize;
    int64 t = getTickCount();
    

    if(argc < 3)
    {
        int rien = 1;
       // print_help_stereo_match();
      // return 0;
    }
    const char* img1_filename = 0;
    string imG_namae = "../data/Image/imageG.jpg";////////////////////
    string imD_namae = "../data/Image/imageD.jpg";/////////////
    const char* img2_filename = 0;
    const char* intrinsic_filename = 0;
    const char* extrinsic_filename = 0;
    const char* disparity_filename = 0;
    const char* point_cloud_filename = 0;

    enum { STEREO_BM=0, STEREO_SGBM=1, STEREO_HH=2, STEREO_VAR=3 };
    int alg = STEREO_BM;
    int SADWindowSize = 0, numberOfDisparities = 0;
    bool no_display = false;
    float scale = 1.f;

    Ptr<StereoBM> bm = StereoBM::create(16,9);
    Ptr<StereoSGBM> sgbm = StereoSGBM::create(0,16,3); // Semi global block matching

    for( int i = 1; i < argc; i++ )
    {
        
         if( strncmp(argv[i], algorithm_opt, strlen(algorithm_opt)) == 0 )
        {
            char* _alg = argv[i] + strlen(algorithm_opt);
            alg = strcmp(_alg, "bm") == 0 ? STEREO_BM :
                  strcmp(_alg, "sgbm") == 0 ? STEREO_SGBM :
                  strcmp(_alg, "hh") == 0 ? STEREO_HH :
                  strcmp(_alg, "var") == 0 ? STEREO_VAR : -1;
            if( alg < 0 )
            {
                printf("Command-line parameter error: Unknown stereo algorithm\n\n");
                print_help_stereo_match();
                return -1;
            }
        }
        else if( strncmp(argv[i], maxdisp_opt, strlen(maxdisp_opt)) == 0 )
        {
            if( sscanf( argv[i] + strlen(maxdisp_opt), "%d", &numberOfDisparities ) != 1 ||
                numberOfDisparities < 1 || numberOfDisparities % 16 != 0 )
            {
                printf("Command-line parameter error: The max disparity (--maxdisparity=<...>) must be a positive integer divisible by 16\n");
                print_help_stereo_match();
                return -1;
            }
        }
        else if( strncmp(argv[i], blocksize_opt, strlen(blocksize_opt)) == 0 )
        {
            if( sscanf( argv[i] + strlen(blocksize_opt), "%d", &SADWindowSize ) != 1 ||
                SADWindowSize < 1 || SADWindowSize % 2 != 1 )
            {
                printf("Command-line parameter error: The block size (--blocksize=<...>) must be a positive odd number\n");
                return -1;
            }
        }
        else if( strncmp(argv[i], scale_opt, strlen(scale_opt)) == 0 )
        {
            if( sscanf( argv[i] + strlen(scale_opt), "%f", &scale ) != 1 || scale < 0 )
            {
                printf("Command-line parameter error: The scale factor (--scale=<...>) must be a positive floating-point number\n");
                return -1;
            }
        }
        else if( strcmp(argv[i], nodisplay_opt) == 0 )
            no_display = true;
        else if( strcmp(argv[i], "-i" ) == 0 )
            intrinsic_filename = argv[++i];
        else if( strcmp(argv[i], "-e" ) == 0 )
            extrinsic_filename = argv[++i];
        else if( strcmp(argv[i], "-o" ) == 0 )
            disparity_filename = argv[++i];
        else if( strcmp(argv[i], "-p" ) == 0 )
            point_cloud_filename = argv[++i];
        else
        {
            printf("Command-line parameter error: unknown option %s\n", argv[i]);
            return -1;
        }
    }

    intrinsic_filename = "../data/intrinsics.yml";
    extrinsic_filename = "../data/extrinsics.yml";

    if( extrinsic_filename == 0 && point_cloud_filename )
    {
        printf("Command-line parameter error: extrinsic and intrinsic parameters must be specified to compute the point cloud\n");
        return -1;
    }

    int color_mode = 0;
    Mat img1 = imread(imG_namae, color_mode);//////////////////
    Mat img2 = imread(imD_namae, color_mode);/////////////////

    if (img1.empty())
    {
        printf("Command-line parameter error: could not load the first input image file\n");
        return -1;
    }
    if (img2.empty())
    {
        printf("Command-line parameter error: could not load the second input image file\n");
        return -1;
    }

    if (scale != 1.f)
    {
        Mat temp1, temp2;
        int method = scale < 1 ? INTER_AREA : INTER_CUBIC;
        resize(img1, temp1, Size(), scale, scale, method);
        img1 = temp1;
        resize(img2, temp2, Size(), scale, scale, method);
        img2 = temp2;
    }

    Size img_size = img1.size();

    Rect roi1, roi2;
    Mat Q;

    if(intrinsic_filename)
    {
        // reading intrinsic parameters
        FileStorage fs(intrinsic_filename, FileStorage::READ);
        if(!fs.isOpened())
        {
            printf("Failed to open file %s\n", intrinsic_filename);
            return -1;
        }
        
        fs["M1"] >> M1;
        fs["D1"] >> D1;
        fs["M2"] >> M2;
        fs["D2"] >> D2;

        M1 *= scale;
        M2 *= scale;

        fs.open(extrinsic_filename, FileStorage::READ);
        if(!fs.isOpened())
        {
            printf("Failed to open file %s\n", extrinsic_filename);
            return -1;
        }
         
        fs["Q"] >> Q;
        fs["R"] >> R;
        fs["T"] >> T;
        fs["R1"] >> R1;
        fs["R2"] >> R2;
        fs["P1"] >> P1;
        fs["P2"] >> P2;
    }

        numberOfDisparities = numberOfDisparities > 0 ? numberOfDisparities : ((img_size.width/8) + 15) & -16;

        bm->setROI1(roi1);
        bm->setROI2(roi2);
        bm->setPreFilterCap(31);
        //int bmWinSize = SADWindowSize > 0 ? SADWindowSize : 3;
        // bm->setBlockSize(bmWinSize);
        bm->setBlockSize(9);
        bm->setMinDisparity(0);
        bm->setNumDisparities(128);
        //bm->setBlockSize(SADWindowSize > 0 ? SADWindowSize : 9);
        // bm->setNumDisparities(numberOfDisparities);
        bm->setTextureThreshold(10);
        bm->setUniquenessRatio(3);
        bm->setSpeckleWindowSize(100);
        bm->setSpeckleRange(32);
        bm->setDisp12MaxDiff(1);

    
    stereoRectify( M1, D1, M2, D2, img_size, R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, -1, img_size, &roi1, &roi2 );

    initUndistortRectifyMap(M1, D1, R1, P1, img_size, CV_16SC2, map11, map12);
    initUndistortRectifyMap(M2, D2, R2, P2, img_size, CV_16SC2, map21, map22);

    //pipeline parameters
        int capture_width = 640; //1280 ;
        int capture_height = 480; //720 ;
        int framerate =  24;
        int display_width = 640; //1280 ;
        int display_height = 480; //720 ;
        


        // Capture the two webcams
        //reset frame average
        std::string pipeline_camLeft = gstreamer_pipeline_camLeft(capture_width, capture_height, framerate,
                                                  display_width, display_height);
        std::string pipeline_camRight = gstreamer_pipeline_camRight(capture_width, capture_height, framerate,
                                                  display_width, display_height);
                                                  
        //
        std::cout << "(CAMLeft) Using pipeline: \n\t" << pipeline_camLeft << "\n\n\n";
        std::cout << "(CAMRight) Using pipeline: \n\t" << pipeline_camRight << "\n\n\n";

        
        VideoCapture capL(pipeline_camLeft, cv::CAP_GSTREAMER);
        VideoCapture capR(pipeline_camRight, cv::CAP_GSTREAMER);

    Mat imgL_gray, imgR_gray;
    Mat temp1, temp2;
    int method = scale < 1 ? INTER_AREA : INTER_CUBIC;

    while(true)
    {
        capL>>img1;
        capR>>img2;

        //preprocess

        resize(img1, temp1, Size(), scale, scale, method);
        img1 = temp1;
        resize(img2, temp2, Size(), scale, scale, method);
        img2 = temp2;

        cv::cvtColor(img1, imgL_gray, cv::COLOR_BGR2GRAY);
        cv::cvtColor(img2, imgR_gray, cv::COLOR_BGR2GRAY);

        img1=imgL_gray;
        img2=imgR_gray;

        remap(img1, img1r, map11, map12, INTER_LINEAR);
        remap(img2, img2r, map21, map22, INTER_LINEAR);

        img1 = img1r;
        img2 = img2r;
   
        bm->compute(img1, img2, disp);
    
        t = getTickCount() - t;
        printf("Time elapsed: %fms\n", t*1000/getTickFrequency());

        disp.convertTo(disp8, CV_8U, 255/(numberOfDisparities*16.));

        namedWindow("disparity", 0);
        imshow("disparity", disp8);
          
        if (cv::waitKey(1) == 27) break;
    }

    return 0;
}
