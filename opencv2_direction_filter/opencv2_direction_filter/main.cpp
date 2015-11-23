#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
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

	//Compute norm of Sobel
	Sobel(image, sobel_x, CV_16S, 1,0);
	Sobel(image, sobel_y, CV_16S, 0, 1);
	Mat sobel_norm;
	sobel_norm = abs(sobel_x) + abs(sobel_y);

	double sobel_min, sobel_max;
	minMaxLoc(sobel_norm, &sobel_min, &sobel_max);
	cout << "sobel value range: " << sobel_min << "  " << sobel_max << endl;

	// Print window pixel values
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++)
			std::cout <<setw(5)<< static_cast<int>(sobel_norm.at<short>(i + 135, j + 362)) << " "; //setw()ÉèÖÃÓò¿í
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	// Conversion to 8 - bit image
	// sobelImage = -alpha*sobel + 255

	Mat sobel_image;
	sobel_norm.convertTo(sobel_image, CV_8U, -255. / sobel_max, 255);
	namedWindow("sobel image", WINDOW_AUTOSIZE);
	imshow("sobel image", sobel_image);

	Mat SobelThreshold;
	threshold(sobel_image, SobelThreshold, 255, 255, THRESH_BINARY);
	namedWindow("sobelThreshold_low", WINDOW_AUTOSIZE);
	imshow("sobelThreshold_low", SobelThreshold);

	threshold(sobel_image, SobelThreshold, 190, 255, THRESH_BINARY);
	namedWindow("sobelThreshold_high", WINDOW_AUTOSIZE);
	imshow("sobelThreshold_high", SobelThreshold);




	



	waitKey(0);
}