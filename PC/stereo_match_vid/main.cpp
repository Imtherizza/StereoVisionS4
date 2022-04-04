/*
 *  stereo_match.cpp
 *  calibration
 *
 *  Created by Victor  Eruhimov on 1/18/10.
 *  Copyright 2010 Argus Corp. All rights reserved.
 *
 */

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/utility.hpp"
//#include <boost/algorithm/string/join.hpp>

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace cv;
using namespace std;

static void print_help()
{
    printf("\nDemo stereo matching converting L and R images into disparity and point clouds\n");
    printf("\nUsage: stereo_match <left_image> <right_image> [--algorithm=bm|sgbm|hh] [--blocksize=<block_size>]\n"
           "[--max-disparity=<max_disparity>] [--scale=scale_factor>] [-i <intrinsic_filename>] [-e <extrinsic_filename>]\n"
           "[--no-display] [-o <disparity_image>] [-p <point_cloud_file>]\n");
}

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

string convertVec(std::vector<cv::String> filename)
{
    std::string s;
    for (const auto &piece : filename) s += piece;
    return s;
}

int main(int argc, char** argv)
{
   
    const char* algorithm_opt = "--algorithm=";
    const char* maxdisp_opt = "--max-disparity=";
    const char* blocksize_opt = "--blocksize=";
    const char* nodisplay_opt = "--no-display";
    const char* scale_opt = "--scale=";

    if(argc < 3)
    {
        int rien = 1;
       // print_help_stereo_match();
      // return 0;
    }
    const char* img1_filename = 0;
    string im1_namae = "/home/stereo-2022/StereoVision/stereo_pics/data/Image/imageG.jpg";////////////////////
    string im2_namae = "/home/stereo-2022/StereoVision/stereo_pics/data/Image/imageD.jpg";/////////////
    const char* img2_filename = 0;
    const char* intrinsic_filename = 0;
    const char* extrinsic_filename = 0;
    const char* disparity_filename = 0;
    const char* point_cloud_filename = 0;

    enum { STEREO_BM=0, STEREO_SGBM=1, STEREO_HH=2, STEREO_VAR=3 };
    int alg = STEREO_SGBM;
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
                print_help();
                return -1;
            }
        }
        else if( strncmp(argv[i], maxdisp_opt, strlen(maxdisp_opt)) == 0 )
        {
            if( sscanf( argv[i] + strlen(maxdisp_opt), "%d", &numberOfDisparities ) != 1 ||
                numberOfDisparities < 1 || numberOfDisparities % 16 != 0 )
            {
                printf("Command-line parameter error: The max disparity (--maxdisparity=<...>) must be a positive integer divisible by 16\n");
                print_help();
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

    if( extrinsic_filename == 0 && point_cloud_filename )
    {
        printf("Command-line parameter error: extrinsic and intrinsic parameters must be specified to compute the point cloud\n");
        return -1;
    }

///////////////////////////////////////////commence iciiiiiiiiiii/////////////////////////////////////////////////////////


/* int color_mode = alg == STEREO_BM ? 0 : -1;
    Mat img1, img2;
    std::string folder1("/home/stereo-2022/Downloads/delivery_area_rig/delivery_area/images/images_rig_cam4/*.png");
    std::string folder2("/home/stereo-2022/Downloads/delivery_area_rig/delivery_area/images/images_rig_cam5/*.png");
    std::vector<cv::String> filenames1, filenames2;
    string filename1, filename2;

    while(true)

     {
///*
        img1 = imread(img1_filename, color_mode);
        img2 = imread(img2_filename, color_mode);
//
  
    cv::glob(folder1, filenames1, false);
    cv::glob(folder2, filenames2, false);

    filename1 = convertVec(filenames1);
    filename2 = convertVec(filenames2);
    cout << filename1;

 //   /*std::string filename1 = boost::algorithm::join(filenames1, ", ");
    std::string filename2 = boost::algorithm::join(filenames1, ", ");

    
    img1 = imread(filename1, color_mode);
    img2 = imread(filename2, color_mode);
///

    
    img1 = imread(filename1, color_mode);
    img2 = imread(filename2, color_mode);

*/

    int color_mode = alg == STEREO_BM ? 0 : -1;
    Mat img1, img2;
    std::string folder1("/home/stereo-2022/Downloads/delivery_area_rig/delivery_area/images/images_rig_cam4/*.png");
    std::string folder2("/home/stereo-2022/Downloads/delivery_area_rig/delivery_area/images/images_rig_cam5/*.png");
    std::vector<cv::String> filenames1, filenames2;
    string filename1, filename2;
    int c = 0;
    int method ;
    Mat temp1, temp2;
    Size img_size ;
    Rect roi1, roi2;
    Mat Q;
   // FileStorage fs(intrinsic_filename, FileStorage::READ);
    Mat R, T, R1, P1, R2, P2;
    Mat map11, map12, map21, map22;
    Mat img1r, img2r;
    int sgbmWinSize;
    int cn; 
    Mat disp, disp8;
    int64 t;
    Mat M1, D1, M2, D2;


    VideoCapture cap1("/home/stereo-2022/Downloads/driveL.mp4"); // PC : "/home/stereo-2022/Downloads/driveL.mp4"     //RASP : "/home/keita/StereoVision/stereo_match_vid/left.mp4"
    VideoCapture cap2("/home/stereo-2022/Downloads/driveR.mp4"); //raspi : "/home/keita/StereoVision/stereo_match_vid/right.mp4"
    
    //VideoCapture cap1(0);
    //VideoCapture cap2(2);
    while(1)

    {

        
        if (cap1.isOpened() == false)
        {
            cout << "cant open the left video"<<endl;
            break;
        }
        
        if (cap2.isOpened() == false)
        {
            cout << "cant open the right video"<<endl;
            break;
        }

        cap1.read(img1);
        cap2.read(img2);
        
        c++;
        cout <<c<<endl;

        //imshow("left",img1);
    // imshow("right",img2);
        
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
                
                method = scale < 1 ? INTER_AREA : INTER_CUBIC;
                resize(img1, temp1, Size(), scale, scale, method);
                img1 = temp1;
                resize(img2, temp2, Size(), scale, scale, method);
                img2 = temp2;
            }

            img_size = img1.size();

            

            if( intrinsic_filename )
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

                
                fs["R"] >> R;
                fs["T"] >> T;

                stereoRectify( M1, D1, M2, D2, img_size, R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, -1, img_size, &roi1, &roi2 );

            
                initUndistortRectifyMap(M1, D1, R1, P1, img_size, CV_16SC2, map11, map12);
                initUndistortRectifyMap(M2, D2, R2, P2, img_size, CV_16SC2, map21, map22);

                
                remap(img1, img1r, map11, map12, INTER_LINEAR);
                remap(img2, img2r, map21, map22, INTER_LINEAR);

                img1 = img1r;
                img2 = img2r;
            }

            numberOfDisparities = numberOfDisparities > 0 ? numberOfDisparities : ((img_size.width/8) + 15) & -16;

            bm->setROI1(roi1);
            bm->setROI2(roi2);
            bm->setPreFilterCap(31);
            //int bmWinSize = SADWindowSize > 0 ? SADWindowSize : 3;
            // bm->setBlockSize(bmWinSize);
            bm->setBlockSize(SADWindowSize > 0 ? SADWindowSize : 9);
            bm->setMinDisparity(0);
            bm->setNumDisparities(numberOfDisparities);
            bm->setTextureThreshold(10);
            bm->setUniquenessRatio(15);
            bm->setSpeckleWindowSize(100);
            bm->setSpeckleRange(32);
            bm->setDisp12MaxDiff(1);

            sgbm->setPreFilterCap(63);
            sgbmWinSize = SADWindowSize > 0 ? SADWindowSize : 3;
            sgbm->setBlockSize(sgbmWinSize);

            cn = img1.channels();

            sgbm->setP1(8*cn*sgbmWinSize*sgbmWinSize);
            sgbm->setP2(32*cn*sgbmWinSize*sgbmWinSize);
            sgbm->setMinDisparity(0);
            sgbm->setNumDisparities(numberOfDisparities);
            sgbm->setUniquenessRatio(10);
            sgbm->setSpeckleWindowSize(100);
            sgbm->setSpeckleRange(32);
            sgbm->setDisp12MaxDiff(1);
            sgbm->setMode(alg == STEREO_HH ? StereoSGBM::MODE_HH : StereoSGBM::MODE_SGBM);

        
            //Mat img1p, img2p, dispp;
            //copyMakeBorder(img1, img1p, 0, 0, numberOfDisparities, 0, IPL_BORDER_REPLICATE);
            //copyMakeBorder(img2, img2p, 0, 0, numberOfDisparities, 0, IPL_BORDER_REPLICATE);

            t = getTickCount();
            if( alg == STEREO_BM )
                bm->compute(img1, img2, disp);
            else if( alg == STEREO_SGBM || alg == STEREO_HH )
                sgbm->compute(img1, img2, disp);
            t = getTickCount() - t;
            printf("Time elapsed: %fms\n", t*1000/getTickFrequency());

            //disp = dispp.colRange(numberOfDisparities, img1p.cols);
            if( alg != STEREO_VAR )
                disp.convertTo(disp8, CV_8U, 255/(numberOfDisparities*16.));
            else
                disp.convertTo(disp8, CV_8U);

            if(disparity_filename)
                    imwrite(disparity_filename, disp8);

            //  namedWindow("left", 1);
            //   imshow("left", img1);
            //    namedWindow("right", 1);
            //  imshow("right", img2);
            //  namedWindow("disparity avant convert", 0);
            //  imshow("disparity avant convert", disp);
               // namedWindow("disparity", 0);
                imshow("disparity", disp8);
            //  printf("sgbmWinSize %d \n",sgbmWinSize);
            // printf("cn %d \n",cn);
            // printf("press any key to continue...");
            //   fflush(stdout);
               // waitKey(20);
                printf("\n");
            //if(disparity_filename)
            //  imwrite(disparity_filename, disp8)



            if (waitKey(10) == 27) break;
    }
    
    return 0;
}
