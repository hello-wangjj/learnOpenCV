#ifndef LINESFINDER_H
#define LINESFINDER_H
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
#define PI 3.1415926
class linesFinder
{
public:
	linesFinder();
	~linesFinder();


	// Set the resolution of the accumulator
	void setAccResolution(double drho, double dTheta){
		deltaTheta=dTheta;
		deltaRho=drho;
	}
	void setminVote(int mVote){
		minVote = mVote;
	}
	void setmLengthAndmGap(double mLength, double mGap){
		minLength = mLength;
		maxGap = mGap;
	}
	vector<Vec4i> findLines(Mat &binary){
		lines.clear();
		HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
		return lines;
	}
	void drawDetectorLines(Mat &image, Scalar color = Scalar(255, 255, 255)){
		vector<Vec4i>::const_iterator it2 = lines.begin();
		while (it2!=lines.end())
		{
			Point pt1((*it2)[0], (*it2)[1]);
			Point pt2((*it2)[2], (*it2)[3]);
			line(image, pt1, pt2, color);
			++it2;
		}
	}

private:
	Mat img;
	vector<Vec4i> lines;
	double deltaRho;
	double deltaTheta;
	int minVote;
	double minLength;
	double maxGap;
};

linesFinder::linesFinder() :deltaRho(1), deltaTheta(PI / 180), minVote(10), minLength(0.), maxGap(0.)
{
}


linesFinder::~linesFinder()
{
}


#endif