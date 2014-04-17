#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;
using namespace std;

int main()
{
	Mat object = imread( "/home/pi/opencv/darkL2.jpg", CV_LOAD_IMAGE_GRAYSCALE );
    //Mat inputImg = imread( "/home/pi/opencv/block.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	//Mat object;
	//resize(inputImg,object,Size(0,0),0.03,0.03);

    if( !object.data )
    {
        cout<< "Error reading object " << endl;
        return -1;
    }
	printf("read image\n");
    //Detect the keypoints using SURF Detector
    int minHessian = 300;

    SurfFeatureDetector detector( minHessian );
    vector<KeyPoint> kp_object;

    detector.detect( object, kp_object );
	//printf("detect keypoints\n");

    //Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;
    Mat des_object;

    extractor.compute( object, kp_object, des_object );

    FlannBasedMatcher matcher;

    //VideoCapture cap(0);
	//VideoCapture cap("/home/pi/opencv/mouse2.mp4" );
	Mat cap = imread( "/home/pi/opencv/photo.jpg", CV_LOAD_IMAGE_COLOR );

	//printf("%f \n",cap.get(CV_CAP_PROP_FPS));

	namedWindow("Good Matches");

    vector<Point2f> obj_corners(4);
	
	//printf("calculate descriptors\n");	
    //Get the corners from the object
    obj_corners[0] = cvPoint(0,0);
    obj_corners[1] = cvPoint( object.cols, 0 );
    obj_corners[2] = cvPoint( object.cols, object.rows );
    obj_corners[3] = cvPoint( 0, object.rows );
	//printf("get corners\n");
    char key = 'a';
    int framecount = 0;

	int angle = 0;
	Mat frame;
	int numCycle = 0;

    while (key != 27)
    {
	/*
	Mat inputVid;	
	Mat frame;
        cap.read(inputVid);
	resize(inputVid, frame, Size(0,0), 0.3,0.3);
	
	
	angle += 10;
	printf("while loop\n");
	int cols = frame.cols;
	int rows = frame.rows;
	Point2f abc(cols/2,rows/2);
	Mat M = getRotationMatrix2D(abc,angle,1);
        warpAffine(cap,frame,M,Size(cols,rows));
	*/
	frame = cap;
        if (framecount < 5 )
        {
            framecount++;
            continue;
        }
	

        Mat des_image, img_matches;
        vector<KeyPoint> kp_image;
        vector<vector<DMatch > > matches;
        vector<DMatch > good_matches;
        vector<Point2f> obj;
        vector<Point2f> scene;
        vector<Point2f> scene_corners(4);
        Mat H;
        Mat image;
	//printf("before color call\n");
        cvtColor(frame, image, CV_BGR2GRAY);

        detector.detect( image, kp_image );
        extractor.compute( image, kp_image, des_image );

        matcher.knnMatch(des_object, des_image, matches, 2);
	//printf("before segfault loop\n");
        for(int i = 0; i < min(des_image.rows-1,(int) matches.size()); i++) //THIS LOOP IS SENSITIVE TO SEGFAULTS
        {
            if((matches[i][0].distance < 0.6*(matches[i][1].distance)) && ((int) matches[i].size()<=2 && (int) matches[i].size()>0))
            {
                good_matches.push_back(matches[i][0]);
            }
        }
	//printf("segfault sensitive loop\n");
        //Draw only "good" matches
        drawMatches( object, kp_object, image, kp_image, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), 						DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	printf("Number of matches: %d\n", good_matches.size());
        if (good_matches.size() >= 10)

        {
		printf("good matches >= 10\n");
            for( int i = 0; i < good_matches.size(); i++ )
            {
                //Get the keypoints from the good matches
                obj.push_back( kp_object[ good_matches[i].queryIdx ].pt );
                scene.push_back( kp_image[ good_matches[i].trainIdx ].pt );
            }

            H = findHomography( obj, scene, CV_RANSAC );

            perspectiveTransform( obj_corners, scene_corners, H);

            //Draw lines between the corners (the mapped object in the scene image )
            line( img_matches, scene_corners[0] + Point2f( object.cols, 0), scene_corners[1] + Point2f( object.cols, 0), Scalar(0, 255, 0), 4 );
            line( img_matches, scene_corners[1] + Point2f( object.cols, 0), scene_corners[2] + Point2f( object.cols, 0), Scalar( 0, 255, 0), 4 );
            line( img_matches, scene_corners[2] + Point2f( object.cols, 0), scene_corners[3] + Point2f( object.cols, 0), Scalar( 0, 255, 0), 4 );
            line( img_matches, scene_corners[3] + Point2f( object.cols, 0), scene_corners[0] + Point2f( object.cols, 0), Scalar( 0, 255, 0), 4 );
        }
	//printf("draw good matches\n");
        //Show detected matches
        imshow( "Good Matches", img_matches );
	if ( numCycle < 20 )
	{
		stringstream name;
		name << numCycle;
		string filename = string("Match_")+name.str()+string(".jpg");
		imwrite( filename, img_matches );
		numCycle++;
	}

        key = waitKey(33);
    }
    return 0;
}
