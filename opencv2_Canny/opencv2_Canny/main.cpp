#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

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

	waitKey(0);
}