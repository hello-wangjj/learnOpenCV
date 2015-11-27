#include <iostream>
#include <opencv2/opencv.hpp>
#include <iomanip>
#include "LinesFinder.h"
#include "circlesFinder.h"
using namespace cv;
using namespace std;
#define  PI 3.1415926

int main()
{
	Mat image = imread("road.jpg", 0);
	if (!image.data) return -1;

	namedWindow("original image", WINDOW_AUTOSIZE);
	imshow("original image", image);

	Mat countours;
	Canny(image, countours, 125, 350);

	namedWindow("Canny image", WINDOW_AUTOSIZE);
	imshow("Canny image", countours);

	Mat countours_inv;
	threshold(countours, countours_inv, 128, 255, THRESH_BINARY_INV);
	namedWindow("Canny image Inv", WINDOW_AUTOSIZE);
	imshow("Canny image Inv", countours_inv);

	Mat sobel_x, sobel_y;
	//Compute norm of Sobel
	Sobel(image, sobel_x, CV_16S, 1, 0);
	Sobel(image, sobel_y, CV_16S, 0, 1);

	//imshow("sobel_y_y", sobel_y);
	Mat sobel_norm;
	sobel_norm = abs(sobel_x) + abs(sobel_y);
	//imshow("sobel norm", sobel_norm);

	double sobel_min, sobel_max;
	minMaxLoc(sobel_norm, &sobel_min, &sobel_max);
	cout << "sobel value range: " << sobel_min << "  " << sobel_max << endl;

	// Print window pixel values
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++)
			std::cout << setw(5) << static_cast<int>(sobel_norm.at<short>(i + 135, j + 362)) << " "; //setw()设置域宽
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
	threshold(sobel_image, SobelThreshold, 180, 255, THRESH_BINARY);
	namedWindow("sobelThreshold_norm", WINDOW_AUTOSIZE);
	imshow("sobelThreshold_norm", SobelThreshold);
	//another 可以sobel之后阈值化
	Mat src = imread("road.jpg",0);
	imshow("src", src);
	Mat x, y;
	Sobel(src, x, CV_16S, 1, 0);
	Sobel(src, y, CV_16S, 0, 1);
	Mat abs_x, abs_y;
	convertScaleAbs(x, abs_x);
	convertScaleAbs(y, abs_y);
	Mat dst;
	dst = abs_y + abs_x;
	threshold(dst, dst, 128, 255, THRESH_BINARY_INV);
	imshow("abs", dst);

	//hough
	vector<Vec2f> lines;
	HoughLines(countours, lines, 1, PI / 180, 80);

	// Draw the lines
	cv::Mat result(countours.rows, countours.cols, CV_8U, cv::Scalar(255));
	image.copyTo(result);

	std::cout << "Lines detected: " << lines.size() << std::endl;

	vector<Vec2f>::const_iterator it = lines.begin();
	while (it!=lines.end())
	{
		float rho = (*it)[0]; //距离 rho
		float theta = (*it)[1]; //角度theta
		if (theta < PI / 4||theta>3.*PI/4.)
		{
			Point pt1(rho / cos(theta),0);
			Point pt2((rho - result.rows*sin(theta)) / cos(theta), result.cols);
			line(image, pt1, pt2, Scalar(255), 1);
		}
		else
		{
			Point pt1(0, rho/sin(theta));
			Point pt2(result.cols, (rho - result.cols*cos(theta)) / sin(theta));
			line(image, pt1, pt2, Scalar(255), 1);
		}
		++it;
	}
	imshow("hough", image);


	//houghP
	linesFinder finder;
	finder.setmLengthAndmGap(100, 20);
	finder.setminVote(80);
	vector<Vec4i> lines2;
	lines2= finder.findLines(countours);
	cout << "lines number:" <<lines2.size()<<endl;
	finder.drawDetectorLines(src);
	namedWindow("hough_p", WINDOW_AUTOSIZE);
	imshow("hough_p", src);

	//houghCircle
	Mat circle = imread("chariot.jpg");
	Mat circle_origin = imread("chariot.jpg");
	circlesFinder finderC;
	finderC.setdpAndmindist(2, 50);
	finderC.setparam(200, 100),
	finderC.setRadius(25, 100);
	vector<Vec3f> circles;
	circles = finderC.findcircles(circle);
	cout << "circles number:" << circles.size() << endl;
	finderC.drawCircles(circle_origin);
	namedWindow("hough_circle", WINDOW_AUTOSIZE);
	imshow("hough_circle", circle_origin);

	waitKey(0);
}