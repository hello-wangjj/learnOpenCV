#ifndef SHARP2D_H
#define SHARP2D_H
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
class sharp2d
{
public:
	sharp2d()=default;
	Mat  sharpen2D(const Mat &image, Mat &result)
	{
		Mat kernel(3, 3, CV_32F, Scalar(0));
		//¸³Öµ
		kernel.at<float>(1, 1) = 5.0;
		kernel.at<float>(0, 1) = -1.0;
		kernel.at<float>(2, 1) = -1.0;
		kernel.at<float>(1, 0) = -1.0;
		kernel.at<float>(1, 2) = -.0;
		filter2D(image, result, image.depth(), kernel);
		return result;
	}
};

#endif