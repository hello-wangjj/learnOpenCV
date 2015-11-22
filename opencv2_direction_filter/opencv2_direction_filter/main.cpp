#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main()
{
	Mat src = imread("boldt.jpg");
	imshow("src", src);
	Mat image = imread("boldt.jpg",0);
	if (!image.data) return 0;

	namedWindow("original img", WINDOW_AUTOSIZE);
	imshow("original img", image);

	Mat sobel_x;
	Sobel(image, sobel_x, CV_8U, 1, 0, 3, 0.4, 128);
	namedWindow("sobel_x", WINDOW_AUTOSIZE);
	//convertScaleAbs(sobel_x, sobel_x);
	imshow("sobel_x", sobel_x);

	Mat sobel_y;
	Sobel(image, sobel_y,CV_8U, 0, 1, 3, 0.4, 128);
	namedWindow("sobel_y", WINDOW_AUTOSIZE);
	//convertScaleAbs(sobel_y, sobel_y);
	imshow("sobel_y", sobel_y);


	waitKey(0);
}