#ifndef CAMERACALIBRATE_H
#define CAMERACALIBRATE_H
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
class camerCalibrate
{
public:
	camerCalibrate();
	~camerCalibrate();
	int addChessboardPoints(const vector <string>&filelist,Size &boardSize)
	{
		// the points on the chessboard
		vector<Point2f> imageCorners;
		vector<Point3f> objectCorners;
		// 3D Scene Points:
		// Initialize the chessboard corners in the chessboard reference frame
		// 
		// The corners are at 3D location (X,Y,Z)= (i,j,0)
		for (int i = 0; i < boardSize.height; i++){
			for (int j = 0; j < boardSize.width;j++)
			{
				objectCorners.push_back(Point3f(i, j, 0.0f));
			}
		}
		//2D Scene Points
		Mat image;// to contain chessboard image
		int success = 0;
		//for all viewpoints
		for (int i = 0; i < filelist.size(); i++){
			image = imread(filelist[i], 0); // Open the image
			// Get the chessboard corners
			bool found = findChessboardCorners(image, boardSize, imageCorners);
			// Get subpixel accuracy on the corners
			cornerSubPix(image, imageCorners, Size(5, 5), Size(-1, -1),TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS,30,0.1));
			if (imageCorners.size() == boardSize.area()){
				addPoints(imageCorners, objectCorners);
				success++;
			}
		}
		return success;
	}
	void addPoints(const vector<Point2f> &imageCorners,const vector<Point3f> &objectCorners){
		//2D points
		imagePoints.push_back(imageCorners);
		objectPoints.push_back(objectCorners);
	}
	// Calibrate the camera
	// returns the re-projection error
	double calibrate(Size &imageSize){
		mustInitUndistort = true;// undistorter must be reinitialized
		vector<Mat> rvecs, tvecs;
		return calibrateCamera(objectPoints, imagePoints, imageSize, camerMatrix, distCoeffs, rvecs, tvecs, flag);

	}
	Mat remap_(Mat &image){
		Mat undistorted;
		if (mustInitUndistort){
			initUndistortRectifyMap(camerMatrix, distCoeffs, Mat(), Mat(), image.size(), CV_32FC1, map1, map2);
			mustInitUndistort = false;
		}
		remap(image, undistorted, map1, map2, INTER_LINEAR);
		return undistorted;
	}

	// Getters
	Mat getCameraMatrix() { return camerMatrix; }
	Mat getDistCoeffs()   { return distCoeffs; }

	void setCalibrationFlag(bool radial8CoeffEnabled, bool tangentialParamEnabled) {

		// Set the flag used in cv::calibrateCamera()
		flag = 0;
		if (!tangentialParamEnabled) flag += CV_CALIB_ZERO_TANGENT_DIST;
		if (radial8CoeffEnabled) flag += CV_CALIB_RATIONAL_MODEL;
	}

private:
	vector<vector<Point3f>>objectPoints;//世界坐标
	vector <vector<Point2f>> imagePoints;//像素坐标
	//输出矩阵
	Mat camerMatrix;
	Mat distCoeffs;
	//标定方式
	int flag;
	//用于图像去畸变
	Mat map1, map2;
	bool mustInitUndistort;

};

camerCalibrate::camerCalibrate() :flag(0), mustInitUndistort(true)
{
}

camerCalibrate::~camerCalibrate()
{
}


#endif