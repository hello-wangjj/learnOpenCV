#include <iostream>
#include <opencv2/opencv.hpp>
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



	waitKey(0);
}