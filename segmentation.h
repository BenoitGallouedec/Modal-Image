#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>
#include "image.h"
#include "test.h"
#include "lineDetector.h"
#include "segmentation.h"


class Segmentation
{
public:
	static void eraseLines(Image<float> src, vector<Vec4f> lines);
	static void segmentation(Image<float> src);
};

