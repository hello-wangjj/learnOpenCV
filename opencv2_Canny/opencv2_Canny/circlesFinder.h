#ifndef CIRCLESFINDERS_H
#define CIRCLESFINDERS_H
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
class circlesFinder
{
public:
	circlesFinder();
	~circlesFinder();

	void setdpAndmindist(double d_p, double mdist){
		dp = d_p;
		minDist = mdist;
	}
	void setparam(double param_1, double param_2){
		param1 = param_1;
		param2 = param_2;
	}
	void setRadius(double miRadius, double maRadius){
		minRadius = miRadius;
		maxRadius = maRadius;
	}
	vector<Vec3f> findcircles(Mat &img){
		//circles.clear();
		GaussianBlur(img, img, Size(5, 5), 1.5);
		cvtColor(img, img, COLOR_BGR2GRAY);
		HoughCircles(img, circles, CV_HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius);
		return circles;
	}
	void drawCircles(Mat &img, Scalar color = Scalar::all(255)){
		vector<Vec3f>::const_iterator itc = circles.begin();
		while (itc!=circles.end())
		{
			circle(img, Point((*itc)[0], (*itc)[1]), (*itc)[2], color, 2);
			++itc;
		}
	}

private:
	Mat image;
	double dp;//用来检测圆心的累加器图像的分辨率与输入图像之比的倒数
	double minDist;
	double param1;
	double param2;
	
	int minRadius;
	int maxRadius;
	vector<Vec3f> circles;
	
};

circlesFinder::circlesFinder() :dp(2), minDist(50.), param1(100.), param2(100.), minRadius(0.), maxRadius(0.)
{
}

circlesFinder::~circlesFinder()
{
}

#endif