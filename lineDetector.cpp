#include "LineDetector.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <iostream>

using namespace cv;
using namespace std;

/*LineDetector::LineDetector(Image<double> *b) :base(b)
{
}*/


LineDetector::~LineDetector()
{
}


vector<Vec4f> LineDetector::Standard_Hough(int threshold, Image<double>& base)
{
	Image<double> srcGray;
	cvtColor(base, srcGray, COLOR_RGB2GRAY);
	Image<double> edges;
	Canny(srcGray, edges, 50, 200, 3);
	vector<Vec2f> s_lines;
	HoughLines(edges, s_lines, 1, CV_PI / 180, threshold, 0, 0);
	vector<Vec4f> lines;
	
	for (size_t i = 0; i < s_lines.size(); i++)
	{
		float r = s_lines[i][0], t = s_lines[i][1];
		double cos_t = cos(t), sin_t = sin(t);
		double x0 = r*cos_t, y0 = r*sin_t;
		double alpha = 1000;
		float x1 = x0 + alpha*(-sin_t);
		float y1 = y0 + alpha*cos_t;
		float x2 = x0 - alpha*(-sin_t);
		float y2 = y0 - alpha*cos_t;

		//On ajoute la ligne uniquement si elle a un sens (on enlève les bugs x1=x2 et y1=y2)
		if (!((x1 == x2) && (y1 == y2))) 
			lines.push_back(Vec4f(x1,y1,x2,y2));
	}
	//Il faut encore trier les lignes par y croissant;
	class linesComparator
	{
	public:
		bool operator()(const Vec4f& a, const Vec4f& b)
		{
			return a[1] < b[1];
		}
	};

	sort(lines.begin(), lines.end(), linesComparator());
	return lines;
}

vector<Vec4f> LineDetector::removeFalseLines(vector<Vec4f> lines){
	double penteMoyenne = 0;
	for (Vec4f a : lines)
		penteMoyenne += (a[3] - a[1]) / (a[2] - a[0]);
	penteMoyenne /= lines.size();
	penteMoyenne = abs(penteMoyenne);
	vector<Vec4f> treatedLines;
	for (Vec4f a : lines) {
		double pente = abs((a[3] - a[1]) / (a[2] - a[0]));
		if (!(abs(penteMoyenne - pente) > 0.001))
			treatedLines.push_back(a);
	}
	return treatedLines;
}

double LineDetector::findRotation(vector<Vec4f> lines) {
	//Le but est de roter l'image afin que les lignes de portée soient planes.
	/*Point2f src_center(source.cols / 2.0F, source.rows / 2.0F);
	Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
	Mat dst;
	warpAffine(source, dst, rot_mat, source.size());*/
	double penteMoyenne=0;
	for (Vec4f a : lines)
		penteMoyenne += (a[3] - a[1]) / (a[2] - a[0]);
	penteMoyenne /= lines.size();

	return atan(penteMoyenne);
}

void LineDetector::showStandardHough(vector<Vec4f>& lines, Image<double>& image,string s) {
	cout << "detect" << endl;
	for (Vec4i v : lines) {
		line(image, Point(cvRound(v(0)), cvRound(v(1))), Point(cvRound(v(2)), cvRound(v(3))), Scalar(255, 0, 0));
	}
	imshow(s, image);
}

vector<vector<Vec4f>> LineDetector::separateStaves(vector<Vec4f> lines) {
	double deltaMax = 0;
	for (int i = 1;i<lines.size();i++) {
		if (abs(lines[i][1] - lines[i - 1][1]) > deltaMax)
			deltaMax = abs(lines[i][1] - lines[i - 1][1]);
	}
	vector<vector<Vec4f>> result;
	vector<Vec4f> stave;
	stave.push_back(lines[0]);
	for (int i = 1; i<lines.size(); i++) {
		if (abs(lines[i][1] - lines[i - 1][1]) > deltaMax / 2) {
			result.push_back(stave);
			stave.clear();
		}
		stave.push_back(lines[i]);
	}
	result.push_back(stave);
	return result;
}

vector<vector<Vec4f>> LineDetector::makeAverageStaves(vector<vector<Vec4f>> lines) {
	vector<vector<Vec4f>> result;
	for (vector<Vec4f> stave : lines) {
		double deltaMax = 0;
		for (int i = 1; i<stave.size(); i++) {
			if (abs(stave[i][1] - stave[i - 1][1]) > deltaMax)
				deltaMax = abs(stave[i][1] - stave[i - 1][1]);
		}
		vector<Vec4f> correctedStave;
		int previousStave = 0;
		for (int i = 1; i<stave.size(); i++) {
			if (abs(stave[i][1] - stave[i - 1][1]) > deltaMax / 2) {//on trouve un écart vrai entre deux lignes
				double averageY = 0;
				for (int j = previousStave; j < i; j++) {
					averageY += stave[j][1];
				}
				averageY /= i - previousStave;
				previousStave = i;
				correctedStave.push_back(Vec4f(stave[i - 1][0], averageY, stave[i-1][2], averageY));
			}
		}
		//Il reste des lignes
		double averageY = 0;
		for (int j = previousStave; j <stave.size(); j++) {
			averageY += stave[j][1];
		}
		averageY /= stave.size() - previousStave;
		correctedStave.push_back(Vec4f(stave[stave.size() - 1][0], averageY, stave[stave.size()-1][2], averageY));
		result.push_back(correctedStave);
	}
	return result;
}

void LineDetector::showFinalStandardHough(vector<vector<Vec4f>>& lines, Image<double>& image, string s) {
	for (int i = 0;i<lines.size();i++) {
		cout << "ok" << endl;
		for (Vec4f vec1 : lines[i]) 
			line(image, Point(cvRound(vec1(0)), cvRound(vec1(1))), Point(vec1(2), vec1(3)), Scalar(255, 0, 0),3);
		
		if (i > 0) {
			int xi = cvRound(((lines[i])[0])(0));
			int xf = cvRound(((lines[i])[0])(2));
			int y = cvRound((((lines[i])[0])(1) + ((lines[i-1])[lines[i-1].size()-1])(1)) / 2);
			cout << "draw line " << xi << " " << xf << " "<<y<<endl;
			line(image, Point(xi, y), Point(xf, y), Scalar(0, 0, 255),5);
		}
	}
	imshow(s, image);
}