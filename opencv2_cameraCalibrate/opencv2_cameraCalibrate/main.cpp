#include <iostream>
#include <opencv2/opencv.hpp>
#include <iomanip>
#include <opencv2/features2d/features2d.hpp>
#include "cameraCalibrate.h"
using namespace cv;
using namespace std;
int main()
{
	Mat image = imread("../chessboards/chessboard07.jpg");
	vector<Point2f> imageCorners;
	Size boardSize(6, 4);
	bool found = findChessboardCorners(image, boardSize, imageCorners);
	drawChessboardCorners(image, boardSize, imageCorners, found);
	imshow("image", image);

	namedWindow("Image");
	Mat Image;
	vector<string> filelist;
	for (int i = 1; i < 20; i++){
		stringstream str1;
		str1 << "../chessboards/chessboard" << setw(2) << setfill('0') << i << ".jpg";
		cout << str1.str() << endl;
		filelist.push_back(str1.str());
		Image = imread(str1.str(), 0);
		imshow("Image", Image);
		waitKey(100);
	}
	camerCalibrate camerCalibrator;
	Size BoardSize(6, 4);
	camerCalibrator.addChessboardPoints(filelist, BoardSize);
	camerCalibrator.calibrate(Image.size());
	// Image Undistortion
	Image = imread(filelist[6]);
	Mat uImage = camerCalibrator.remap_(Image);
	// display camera matrix
	Mat camerMatrix = camerCalibrator.getCameraMatrix();
	std::cout << " Camera intrinsic: " << camerMatrix.rows << "x" << camerMatrix.cols << std::endl;
	std::cout << camerMatrix.at<double>(0, 0) << " " << camerMatrix.at<double>(0, 1) << " " << camerMatrix.at<double>(0, 2) << std::endl;
	std::cout << camerMatrix.at<double>(1, 0) << " " << camerMatrix.at<double>(1, 1) << " " << camerMatrix.at<double>(1, 2) << std::endl;
	std::cout << camerMatrix.at<double>(2, 0) << " " << camerMatrix.at<double>(2, 1) << " " << camerMatrix.at<double>(2, 2) << std::endl;

	imshow("Original Image", Image);
	imshow("Undistorted Image", uImage);


	waitKey(0);
	return 0;
}