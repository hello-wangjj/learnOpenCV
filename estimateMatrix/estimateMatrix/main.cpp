#include "estimate.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
int main()
{
	Mat image1 = imread("../church01.jpg",0);
	Mat image2 = imread("church03.jpg", 0);
	namedWindow("Right Image");
	imshow("Right Image", image1);
	namedWindow("Left Image");
	imshow("Left Image", image2);

	vector<KeyPoint>keypoints1, keypoints2;
	SurfFeatureDetector surf(3000);

	surf.detect(image1, keypoints1);
	surf.detect(image2, keypoints2);

	std::cout << "Number of SURF points (1): " << keypoints1.size() << std::endl;
	std::cout << "Number of SURF points (2): " << keypoints2.size() << std::endl;

	Mat keypointImg1, keypointImg2;
	drawKeypoints(image1, keypoints1, keypointImg1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("Right keypoint", keypointImg1);
	drawKeypoints(image2, keypoints2, keypointImg2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("Left keypoint", keypointImg2);

	SurfDescriptorExtractor surfdesc;
	Mat descriptor1, descriptor2;
	surfdesc.compute(image1, keypoints1, descriptor1);
	surfdesc.compute(image2, keypoints2, descriptor2);

	std::cout << "descriptor matrix size: " << descriptor1.rows << " by " << descriptor1.cols << std::endl;
	std::cout << "descriptor matrix size: " << descriptor2.rows << " by " << descriptor2.cols << std::endl;

	BruteForceMatcher<L2<float>> matcher;
	vector<DMatch> matches;
	matcher.match(descriptor1, descriptor2, matches);
	std::cout << "Number of matched points: " << matches.size() << std::endl;
	Mat imaMatches;
	nth_element(matches.begin(), matches.begin() + 24, matches.end());
	matches.erase(matches.begin() + 25, matches.end());


	drawMatches(image1, keypoints1, image2, keypoints2, matches, imaMatches, Scalar::all(-1));
	imshow("matches", imaMatches);


	// Convert 1 vector of keypoints into
	// 2 vectors of Point2f
	vector<int> pointIndexes1;
	vector<int> pointIndexes2;
	for (std::vector<cv::DMatch>::const_iterator it = matches.begin();
		it != matches.end(); ++it) {

		// Get the indexes of the selected matched keypoints
		pointIndexes1.push_back(it->queryIdx);
		pointIndexes2.push_back(it->trainIdx);
	}
	vector<cv::Point2f> selPoints1, selPoints2;
	cv::KeyPoint::convert(keypoints1, selPoints1, pointIndexes1);
	cv::KeyPoint::convert(keypoints2, selPoints2, pointIndexes2);

	// check by drawing the points 
	std::vector<cv::Point2f>::const_iterator it = selPoints1.begin();
	while (it != selPoints1.end()) {

		// draw a circle at each corner location
		cv::circle(image1, *it, 3, cv::Scalar(255, 255, 255), 2);
		++it;
	}

	it = selPoints2.begin();
	while (it != selPoints2.end()) {

		// draw a circle at each corner location
		cv::circle(image2, *it, 3, cv::Scalar(255, 255, 255), 2);
		++it;
	}

	// Compute F matrix from 7 matches
	cv::Mat fundemental = cv::findFundamentalMat(
		cv::Mat(selPoints1), // points in first image
		cv::Mat(selPoints2), // points in second image
		CV_FM_7POINT);       // 7-point method

	std::cout << "F-Matrix size= " << fundemental.rows << "," << fundemental.cols << std::endl;

	// draw the left points corresponding epipolar lines in right image 
	std::vector<cv::Vec3f> lines1;
	cv::computeCorrespondEpilines(
		cv::Mat(selPoints1), // image points 
		1,                   // in image 1 (can also be 2)
		fundemental, // F matrix
		lines1);     // vector of epipolar lines

	// for all epipolar lines
	for (vector<cv::Vec3f>::const_iterator it = lines1.begin();
		it != lines1.end(); ++it) {

		// draw the epipolar line between first and last column
		cv::line(image2, cv::Point(0, -(*it)[2] / (*it)[1]),
			cv::Point(image2.cols, -((*it)[2] + (*it)[0] * image2.cols) / (*it)[1]),
			cv::Scalar(255, 255, 255));
	}

	// draw the left points corresponding epipolar lines in left image 
	std::vector<cv::Vec3f> lines2;
	cv::computeCorrespondEpilines(cv::Mat(selPoints2), 2, fundemental, lines2);
	for (vector<cv::Vec3f>::const_iterator it = lines2.begin();
		it != lines2.end(); ++it) {

		// draw the epipolar line between first and last column
		cv::line(image1, cv::Point(0, -(*it)[2] / (*it)[1]),
			cv::Point(image1.cols, -((*it)[2] + (*it)[0] * image1.cols) / (*it)[1]),
			cv::Scalar(255, 255, 255));
	}

	// Display the images with points and epipolar lines
	cv::namedWindow("Right Image Epilines");
	cv::imshow("Right Image Epilines", image1);
	cv::namedWindow("Left Image Epilines");
	cv::imshow("Left Image Epilines", image2);




	waitKey(0);
	return 0;
}