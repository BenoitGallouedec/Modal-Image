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

vector<Vec2f> Standard_Hough(Image<float>& edges,int threshold)
{
	vector<Vec2f> s_lines;
	HoughLines(edges, s_lines, 1, CV_PI / 180, threshold, 0, 0);
	return s_lines;
}

void showStandardHough(vector<Vec2f>& s_lines,Image<float>& imageI) {
	Image<float> image(imageI);
	for (size_t i = 0; i < s_lines.size(); i++)
	{
		float r = s_lines[i][0], t = s_lines[i][1];
		double cos_t = cos(t), sin_t = sin(t);
		double x0 = r*cos_t, y0 = r*sin_t;
		double alpha = 1000;

		Point pt1(cvRound(x0 + alpha*(-sin_t)), cvRound(y0 + alpha*cos_t));
		Point pt2(cvRound(x0 - alpha*(-sin_t)), cvRound(y0 - alpha*cos_t));
		line(image, pt1, pt2, Scalar(255, 0, 0), 1, LINE_AA);
	}
	imshow("test", image);
}

/**
* @function Probabilistic_Hough
*/
vector<Vec4i> Probabilistic_Hough(Image<float>& edges, int threshold)
{
	Image<float> processedImage;
	vector<Vec4i> p_lines;
	cvtColor(edges, processedImage, COLOR_GRAY2BGR);
	HoughLinesP(edges, p_lines, 1, CV_PI / 180, threshold, 30, 10);
	return p_lines;
}

void showProbabilisticHough(vector<Vec4i>& p_lines, Image<float>& processedImage) {
	Image<float> b = processedImage;
	for (size_t i = 0; i < p_lines.size(); i++)
	{
		Vec4i l = p_lines[i];
		line(b, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 3, LINE_AA);
	}
	imshow("Méthode probabiliste", b);
}

void onTrackbarP(int sigma, void* p)
{
	cout << sigma << endl;
	Image<float> A = *(Image<float>*)p;
	Image<float> B;
	showStandardHough(Standard_Hough(A, sigma), A);
}