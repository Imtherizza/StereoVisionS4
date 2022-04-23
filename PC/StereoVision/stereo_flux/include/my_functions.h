#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ximgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/videoio/videoio.hpp"

#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


using namespace cv;
using namespace std;

//
String inttostr(int input);

//Prends la photo a traiter ou les photos necessaires a la calibration es les ranges dans les bon dossiers
//genere un fichier xml pour la calibration
int snaps (bool skip);

//
static int print_help_calib();

//
static void StereoCalib(bool skip, const vector<string>& imagelist, Size boardSize,bool displayCorners, bool useCalibrated, bool showRectified);

//
int stereo_calib (int argc, char** argv);

//
static void print_help_stereo_match();

//
static void saveXYZ(const char* filename, const Mat& mat);

//
int stereo_match(int argc, char** argv);























