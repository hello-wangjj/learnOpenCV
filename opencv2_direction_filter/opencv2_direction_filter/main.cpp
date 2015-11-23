#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include "laplacian.h"
#include "sharpen2D.h"
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

	//imshow("sobel_y_y", sobel_y);
	Mat sobel_norm;
	sobel_norm = abs(sobel_x) + abs(sobel_y);

	double sobel_min, sobel_max;
	minMaxLoc(sobel_norm, &sobel_min, &sobel_max);
	cout << "sobel value range: " << sobel_min << "  " << sobel_max << endl;

	// Print window pixel values
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++)
			std::cout <<setw(5)<< static_cast<int>(sobel_norm.at<short>(i + 135, j + 362)) << " "; //setw()设置域宽
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

	threshold(sobel_image, SobelThreshold, 125, 255, THRESH_BINARY);
	namedWindow("sobelThreshold_mid", WINDOW_AUTOSIZE);
	imshow("sobelThreshold_mid", SobelThreshold);

	//distance and direction
	Mat sobel_x1, sobel_y1;
	Sobel(image, sobel_x1, CV_32F, 1, 0);
	Sobel(image, sobel_y1, CV_32F, 0, 1);
	Mat norm, dir;
	cartToPolar(sobel_x1, sobel_y1, norm, dir);
	threshold(norm, norm, 190, 255, THRESH_BINARY);
	imshow("norm", norm);

	// Print window pixel values
	/*/不会输出dir
	/for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++)
			std::cout << setw(5) << static_cast<int>(dir.at<short>(i + 135, j + 362)) << " "; //setw()设置域宽
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	//cout << dir << endl;*/

	// Compute Laplacian 
	Mat laplace;

	Laplacian(image, laplace, CV_8U, 1, 1, 128);
	namedWindow("Laplacian Image");
	imshow("Laplacian Image", laplace);

	Laplacian(image, laplace, CV_8U, 7,0.01, 128);
	namedWindow("Laplacian Image1");
	imshow("Laplacian Image1", laplace);

	// Print window pixel values
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++)
			std::cout << std::setw(5) << static_cast<int>(laplace.at<uchar>(i + 135, j + 362)) - 128 << " ";
		std::cout << std::endl;
	}

	Laplacian(image, laplace, CV_8U, 7, 1, 128);
	namedWindow("Laplacian Image2");
	imshow("Laplacian Image2", laplace);
	// Extract small window
	cv::Mat window(image, cv::Rect(362, 135, 12, 12));
	cv::namedWindow("Image window");
	cv::imshow("Image window", window);
	cv::imwrite("window.bmp", window);


	LaplacianZC laplacian;
	laplacian.setAperture(7);
	Mat flap = laplacian.computeLaplacian(image);
	laplace = laplacian.getLaplacianImage();
	namedWindow("laplacian Img", WINDOW_AUTOSIZE);
	imshow("laplacian Img", laplace);

	double lapmin, lapmax;
	minMaxLoc(flap, &lapmin, &lapmax);
	std::cout << "Laplacian value range=[" << lapmin << "," << lapmax << "]\n";
	std::cout << std::endl;
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++)
			std::cout << std::setw(5) << static_cast<int>(flap.at<float>(i + 135, j + 362) / 100) << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	// Compute and display the zero-crossing points
	Mat zeros;
	zeros = laplacian.getZeroCrossings(lapmax);
	namedWindow("Zero-crossings");
	imshow("Zero-crossings", zeros);

	// Compute and display the zero-crossing points (Sobel version)
	zeros = laplacian.getZeroCrossings();
	zeros = laplacian.getZeroCrossingsWithSobel(50);
	cv::namedWindow("Zero-crossings (2)");
	cv::imshow("Zero-crossings (2)", zeros);

	// Print window pixel values
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++)
			std::cout << std::setw(2) << static_cast<int>(zeros.at<uchar>(i + 135, j + 362)) << " ";
		std::cout << std::endl;
	}

	// Display the image with window
	cv::rectangle(image, cv::Point(362, 135), cv::Point(374, 147), cv::Scalar(255, 255, 255));
	cv::namedWindow("Original Image with window");
	cv::imshow("Original Image with window", image);

	sharp2d H1;
	Mat sharpImg;
	H1.sharpen2D(src,sharpImg);
	namedWindow("sharpImg");
	imshow("sharpImg", sharpImg);

	

	waitKey(0);
}