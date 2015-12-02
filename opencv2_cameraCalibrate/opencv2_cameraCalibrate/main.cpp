#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main()
{
	Mat image = imread("chessboard07.jpg");
	vector<Point2f> imageCorners;
	Size boardSize(6, 4);
	bool found = findChessboardCorners(image, boardSize, imageCorners);
	drawChessboardCorners(image, boardSize, imageCorners, found);
	imshow("image", image);
	waitKey(0);
	return 0;
}