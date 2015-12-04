#include <opencv2/opencv.hpp>
#include <iostream>
#include "RobusMatch.h"
using namespace std;
using namespace cv;
int main()
{
	cv::Mat image1 = cv::imread("canal1.jpg", 0);
	cv::Mat image2 = cv::imread("canal2.jpg", 0);
	if (!image1.data || !image2.data)
		return 0;

	// Display the images
	cv::namedWindow("Right Image");
	cv::imshow("Right Image", image1);
	cv::namedWindow("Left Image");
	cv::imshow("Left Image", image2);

	// Prepare the matcher
	RobusMatcher rmatcher;
	rmatcher.setConfidenceLevel(0.98);
	rmatcher.setMinDistanceToEpipolar(1.0);
	rmatcher.setRatio(0.65f);
	cv::Ptr<cv::FeatureDetector> pfd = new cv::SurfFeatureDetector(10);
	rmatcher.setFeatureDetector(pfd);

	// Match the two images
	std::vector<cv::DMatch> matches;
	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat fundemental = rmatcher.match(image1, image2, matches, keypoints1, keypoints2);

	// draw the matches
	cv::Mat imageMatches;
	cv::drawMatches(image1, keypoints1,  // 1st image and its keypoints
		image2, keypoints2,  // 2nd image and its keypoints
		matches,			// the matches
		imageMatches,		// the image produced
		cv::Scalar(255, 255, 255)); // color of the lines
	cv::namedWindow("Matches");
	cv::imshow("Matches", imageMatches);

	// Convert keypoints into Point2f	
	std::vector<cv::Point2f> points1, points2;

	for (std::vector<cv::DMatch>::const_iterator it = matches.begin();
		it != matches.end(); ++it) {

		// Get the position of left keypoints
		float x = keypoints1[it->queryIdx].pt.x;
		float y = keypoints1[it->queryIdx].pt.y;
		points1.push_back(cv::Point2f(x, y));
		cv::circle(image1, cv::Point(x, y), 3, cv::Scalar(255, 255, 255), 3);
		// Get the position of right keypoints
		x = keypoints2[it->trainIdx].pt.x;
		y = keypoints2[it->trainIdx].pt.y;
		cv::circle(image2, cv::Point(x, y), 3, cv::Scalar(255, 255, 255), 3);
		points2.push_back(cv::Point2f(x, y));
	}

	// Draw the epipolar lines
	std::vector<cv::Vec3f> lines1;
	cv::computeCorrespondEpilines(cv::Mat(points1), 1, fundemental, lines1);

	for (vector<cv::Vec3f>::const_iterator it = lines1.begin();
		it != lines1.end(); ++it) {

		cv::line(image2, cv::Point(0, -(*it)[2] / (*it)[1]),
			cv::Point(image2.cols, -((*it)[2] + (*it)[0] * image2.cols) / (*it)[1]),
			cv::Scalar(255, 255, 255));
	}

	std::vector<cv::Vec3f> lines2;
	cv::computeCorrespondEpilines(cv::Mat(points2), 2, fundemental, lines2);

	for (vector<cv::Vec3f>::const_iterator it = lines2.begin();
		it != lines2.end(); ++it) {

		cv::line(image1, cv::Point(0, -(*it)[2] / (*it)[1]),
			cv::Point(image1.cols, -((*it)[2] + (*it)[0] * image1.cols) / (*it)[1]),
			cv::Scalar(255, 255, 255));
	}

	// Display the images with epipolar lines
	cv::namedWindow("Right Image Epilines (RANSAC)");
	cv::imshow("Right Image Epilines (RANSAC)", image1);
	cv::namedWindow("Left Image Epilines (RANSAC)");
	cv::imshow("Left Image Epilines (RANSAC)", image2);

	cv::waitKey(0);
	return 0;
}