#pragma once
#include "image.h"
#include <cmath>
class LineDetector
{
public:

	static vector<Vec4f> Standard_Hough(int threshold, Image<double>& b);
	static vector<Vec4f> removeFalseLines(vector<Vec4f> lines);
	static double findRotation(vector<Vec4f> lines);
	static void showStandardHough(vector<Vec4f>& lines, Image<double>& image,string s);
	static void showFinalStandardHough(vector<vector<Vec4f>>& lines, Image<double>& image, string s);
	static vector<vector<Vec4f>> separateStaves(vector<Vec4f> lines);
	static vector<vector<Vec4f>> makeAverageStaves(vector<vector<Vec4f>> lines);
};

