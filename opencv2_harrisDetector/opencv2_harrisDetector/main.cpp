#include <iostream>
#include <opencv2/opencv.hpp>
//#include <features2d/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
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
	//Mat dstimg(image0.size(),CV_8U);
	drawKeypoints(image0, keypoints, image0, Scalar::all(-1), DrawMatchesFlags::DRAW_OVER_OUTIMG);
	imshow("feature", image0);
	//SURF
	Mat image1 = imread("church01.jpg");
	vector<KeyPoint> keypoints1;
	SurfFeatureDetector surf(2500.);
	surf.detect(image1, keypoints1);
	drawKeypoints(image1, keypoints1, image1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("surf",image1);

	Mat image2 = imread("church01.jpg");
	vector<KeyPoint> keypoints2;
	SiftFeatureDetector sift(0.03,10.);
	sift.detect(image2, keypoints2);
	drawKeypoints(image2, keypoints2, image2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	imshow("sift", image2);






	waitKey(0);
}