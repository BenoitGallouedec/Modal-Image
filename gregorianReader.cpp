#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "image.h"
#include "test.h"
#include "lineDetector.h"
#include "segmentation.h"
#include <cmath>
using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
//void thresh_callback(int, void*);

/** @function main */
/*int main(int argc, char** argv)
{
	/// Load source image and convert it to gray
	src = imread("vertical.jpg", 1);

	/// Convert image to gray and blur it
	cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));

	/// Create Window
	char* source_window = "Source";
	namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	imshow(source_window, src);

	createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, thresh_callback);
	thresh_callback(0, 0);

	waitKey(0);
	return(0);
}

/** @function thresh_callback */
 /*void thresh_callback(int, void*)
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	imwrite("result.jpg", drawing);
}*/
 //////////////////////////////////

int main() {
	Image<double> src(imread("testoblique.jpg", IMREAD_COLOR));
	Image<double> src1(imread("testoblique.jpg", IMREAD_COLOR));
	vector<Vec4f> v = LineDetector::Standard_Hough(180,src);
	LineDetector::showStandardHough(v, src1,"t");
	vector<Vec4f> v1 = LineDetector::removeFalseLines(v);
	waitKey(0);
	Image<double> src2(imread("testoblique.jpg", IMREAD_COLOR));
	double rotation = LineDetector::findRotation(v1);
	LineDetector::showStandardHough(v1, src2,"u");
	waitKey(0);
	Mat transform_m = getRotationMatrix2D(Point2f(src.width() / 2, src.height() / 2), rotation*360/2/3.14159, 1);  //Creating rotation matrix
	Image<double>  dst;
	warpAffine(src, dst, transform_m, src.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(255, 255, 255));
	vector<Vec4f> v2 = LineDetector::Standard_Hough(180,dst);
	vector<Vec4f> v3 = LineDetector::removeFalseLines(v2);
	LineDetector::showStandardHough(v3, dst, "w");
	vector<vector<Vec4f>> staves = LineDetector::separateStaves(v3);
	vector<vector<Vec4f>> corrected = LineDetector::makeAverageStaves(staves);
	waitKey(0);
	Image<double> src4(imread("testoblique.jpg", IMREAD_COLOR));
	Image<double>  dst1;
	warpAffine(src4, dst1, transform_m, src.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(255, 255, 255));
	//LineDetector::showFinalStandardHough(corrected, dst1, "e");
	Segmentation::eraseLines(dst1,v3);
	Segmentation::segmentation(dst1);
	waitKey(0);
	return 0;
}
/*
int main(int, char** argv)
{
	// Load the image
	Mat src = imread("test2.jpg");
	// Check if image is loaded fine
	if (!src.data)
		cerr << "Problem loading image!!!" << endl;
	// Show source image
	imshow("src", src);
	// Transform source image to gray if it is not
	Mat gray;
	if (src.channels() == 3)
	{
		cvtColor(src, gray, CV_BGR2GRAY);
	}
	else
	{
		gray = src;
	}
	// Show gray image
	imshow("gray", gray);
	// Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
	Mat bw;
	adaptiveThreshold(~gray, bw, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
	// Show binary image
	imshow("binary", bw);
	// Create the images that will use to extract the horizontal and vertical lines
	Mat horizontal = bw.clone();
	Mat vertical = bw.clone();
	// Specify size on horizontal axis
	int horizontalsize = horizontal.cols / 30;
	// Create structure element for extracting horizontal lines through morphology operations
	Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize, 1));
	// Apply morphology operations
	erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
	dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
	// Show extracted horizontal lines
	imshow("horizontal", horizontal);
	// Specify size on vertical axis
	int verticalsize = vertical.rows / 100;
	// Create structure element for extracting vertical lines through morphology operations
	Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, verticalsize));
	// Apply morphology operations
	erode(vertical, vertical, verticalStructure, Point(-1, -1));
	dilate(vertical, vertical, verticalStructure, Point(-1, -1));
	// Show extracted vertical lines
	imshow("vertical", vertical);
	// Inverse vertical image
	bitwise_not(vertical, vertical);
	imshow("vertical_bit", vertical);
	// Extract edges and smooth image according to the logic
	// 1. extract edges
	// 2. dilate(edges)
	// 3. src.copyTo(smooth)
	// 4. blur smooth img
	// 5. smooth.copyTo(src, edges)
	// Step 1
	Mat edges;
	adaptiveThreshold(vertical, edges, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);
	imshow("edges", edges);
	// Step 2
	Mat kernel = Mat::ones(2, 2, CV_8UC1);
	dilate(edges, edges, kernel);
	imshow("dilate", edges);
	// Step 3
	Mat smooth;
	vertical.copyTo(smooth);
	// Step 4
	blur(smooth, smooth, Size(2, 2));
	// Step 5
	smooth.copyTo(vertical, edges);
	// Show final result
	imshow("smooth", vertical);
	imwrite("test4.jpg",smooth);
	waitKey(0);
	return 0;
}*/