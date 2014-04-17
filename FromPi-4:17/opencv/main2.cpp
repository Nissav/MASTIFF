#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <Python.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;
using namespace std;
bool findMatch(CvPoint &offset, FlannBasedMatcher matcher, SurfFeatureDetector detector, SurfDescriptorExtractor extractor, Mat des_object[]);
int main(int argc, char** argv)
{
	printf("BEGIN\n");
	PyObject *pName, *pModule, *pDict, *pFunc, *pValue;
	printf("BEGIN\n");
	Py_Initialize();
	PySys_SetArgv(argc, argv);
	printf("BEGIN\n");
	//PySys_SetPath("/home/pi/opencv");
	//printf("BEGIN\n");
	pName = PyString_FromString(argv[1]);
	printf("BEGIN\n");
	pModule = PyImport_Import(pName);
	printf("BEGIN\n");
	pDict = PyModule_GetDict(pModule);
	printf("BEGIN\n");
	pFunc = PyDict_GetItemString(pDict, argv[2]);
	printf("END\n");
	Mat objects[4];
	Mat object1 = imread( "/home/pi/opencv/stockPhotos/sstock1.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	Mat object2 = imread( "/home/pi/opencv/stockPhotos/sstock3.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	Mat object3 = imread( "/home/pi/opencv/stockPhotos/phone3.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	Mat object4 = imread( "/home/pi/opencv/stockPhotos/phone2.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	objects[0] = object1;
	objects[1] = object2;
	objects[2] = object3;
	objects[3] = object4;

	int minHessian = 600;
	SurfFeatureDetector detector( minHessian );
    	vector<KeyPoint> kp_object;
	
	SurfDescriptorExtractor extractor;
   	Mat des_object[4];
	FlannBasedMatcher matcher;
	Mat object;
	for ( int k = 0 ; k < 4 ; k++ )
	{
		object = objects[k];
		detector.detect( object, kp_object );
		extractor.compute( object, kp_object, des_object[k]);
	}
	bool noMatch = true;
	CvPoint offset = cvPoint(0,0);
	while(noMatch){
		PyObject_CallObject(pFunc, NULL);
		noMatch = findMatch(offset, matcher, detector, extractor, des_object);
	} 
	printf("Offset from the center:\nx = %d pixels to the right\ny = %d pixels below\n", offset.x, offset.y);

	cout << offset.x << "," << offset.y << endl;
	Py_DECREF(pModule);
	Py_DECREF(pName);
	Py_Finalize();
    	return 0;
}

bool findMatch(CvPoint &offset, FlannBasedMatcher matcher, SurfFeatureDetector detector, SurfDescriptorExtractor extractor, Mat des_object[])
{
	bool noMatch = true;
	Mat des_image, img_matches;
	vector<KeyPoint> kp_image;
	vector<vector<DMatch > > matches;
	vector<DMatch > good_matches;
	int iter = 0;
	Mat image = imread("/home/pi/opencv/photo.jpg" , CV_LOAD_IMAGE_GRAYSCALE );
	detector.detect( image, kp_image );
	extractor.compute( image, kp_image, des_image );
	while ( noMatch )
	{
		//printf("before kp and des detection 2\n");
	    	
		
		matcher.knnMatch(des_object[iter], des_image, matches, 2);
		for(int i = 0; i < min(des_image.rows-1,(int) matches.size()); i++) //THIS LOOP IS SENSITIVE TO SEGFAULTS
		{
		    if((matches[i][0].distance < 0.6*(matches[i][1].distance)) && ((int) matches[i].size()<=2 && (int) matches[i].size()>0))
		    {
			good_matches.push_back(matches[i][0]);
		    }
		}
		
		//printf("Number of matches: %d\n", good_matches.size());
		if (good_matches.size() >= 10)
		{
			CvPoint center = cvPoint(0,0);
			for ( int z = 0 ; z < good_matches.size() ; z++ )
			{
				int index = good_matches.at(z).trainIdx;
				center.x += kp_image.at(index).pt.x;
				center.y += kp_image.at(index).pt.y;
			}
			center.x = center.x/good_matches.size();
			center.y = center.y/good_matches.size();
			int radius = 5;
			circle( image, center, radius, {0,0,255}, 3, 8, 0 );
			namedWindow("test");
			imshow("test", image);
			imwrite("centerPoint.jpg", image);
			waitKey(5000);
			int offsetX = center.x - image.cols/2;
			int offsetY = center.y - image.rows/2;
			offset = cvPoint(offsetX, offsetY);			
			noMatch = false;
		}
		//printf("draw good matches\n");
		//Show detected matches
		if ( iter++ == 3 || !noMatch )
			break;
		
		good_matches.clear();
	}
	return noMatch;
}
