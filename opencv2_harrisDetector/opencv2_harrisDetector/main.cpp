#include <iostream>
#include <opencv2/opencv.hpp>
#include <features2d/features2d.hpp>
#include "harrisDetector.h"
using namespace cv;
using namespace std;
int main()
{
	Mat image = imread("church01.jpg",0);
	imshow("church", image);

	Mat cornerStrength;
	cornerHarris(image, cornerStrength, 3, 3, 0.01);
	Mat harrisCorners;
	double threshold1 = 0.0001;
	threshold(cornerStrength, harrisCorners, threshold1, 255, THRESH_BINARY_INV);
	imshow("harrisCorner", harrisCorners);

	//构造的函数
	harrisDetector harris;
	harris.detect(image);
	vector<Point> pts;
	harris.getCorners(pts, 0.01);
	harris.drawOnImg(image,pts);
	imshow("harris", image);

	//FAST
	Mat image0 = imread("church01.jpg");
	vector<KeyPoint> keypoints;
	FastFeatureDetector fast(50);
	fast.detect(image0, keypoints);
	drawKeypoints(image0, keypoints, image0, Scalar::all(-1), DrawMatchesFlags::DRAW_OVER_OUTIMG);
	imshow("feature", image0);




	waitKey(0);
}