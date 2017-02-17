/**
* @file HoughLines_Demo.cpp
* @brief Demo code for Hough Transform
* @author OpenCV team
*/

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "image.h"
using namespace cv;
using namespace std;

vector<Vec2f> Standard_Hough(Image<float>& edges, int threshold);

void showStandardHough(vector<Vec2f>& s_lines, Image<float>& image);

/**
* @function Probabilistic_Hough
*/
vector<Vec4i> Probabilistic_Hough(Image<float>& edges, int threshold);

void showProbabilisticHough(vector<Vec4i>& p_lines, Image<float>& processedImage);

void onTrackbarP(int sigma, void* p);
