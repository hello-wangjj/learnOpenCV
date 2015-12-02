#include <iostream>
#include <opencv2/opencv.hpp>
//#include <features2d/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
#include "harrisDetector.h"
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

	//构造的函数
	harrisDetector harris;
	harris.detect(image);
	vector<Point> pts;
	harris.getCorners(pts, 0.01);
	harris.drawOnImg(image,pts);
	imshow("harris", image);

	//FAST
	Mat image0 = imread("church01.jpg");
	vector<KeyPoint> keypoints;
	FastFeatureDetector fast(50);
	fast.detect(image0, keypoints);
	//Mat dstimg(image0.size(),CV_8U);
	drawKeypoints(image0, keypoints, image0, Scalar::all(-1), DrawMatchesFlags::DRAW_OVER_OUTIMG);
	imshow("feature", image0);
	//SURF
	Mat image1 = imread("church01.jpg");
	vector<KeyPoint> keypoints1;
	SurfFeatureDetector surf(2500.);
	surf.detect(image1, keypoints1);
	drawKeypoints(image1, keypoints1, image1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("surf",image1);

	Mat image2 = imread("church01.jpg");
	vector<KeyPoint> keypoints2;
	SiftFeatureDetector sift(0.03,10.);
	sift.detect(image2, keypoints2);
	drawKeypoints(image2, keypoints2, image2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("sift", image2);


	//describe SURF
	Mat image3 = imread("church01.jpg");
	Mat image4= imread("church02.jpg");
	SurfDescriptorExtractor surfExt;
	SurfFeatureDetector surfDesc(3000);
	Mat descriptors1,descriptors2;
	vector<KeyPoint>keypoints3,keypoints4;

	surfDesc.detect(image3, keypoints3);
	surfDesc.detect(image4, keypoints4);
	Mat dstimg3, dstimg4;
	drawKeypoints(image3, keypoints3, dstimg3, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("dst3", dstimg3);
	drawKeypoints(image4, keypoints4, dstimg4, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("dst4", dstimg4);
	

	surfExt.compute(image3, keypoints3, descriptors1);
	surfExt.compute(image4, keypoints4, descriptors2);

	std::cout << "descriptor matrix size: " << descriptors1.rows << " by " << descriptors1.cols << std::endl;

	BruteForceMatcher<L2<float>> matcher;
	vector<DMatch>matches;
	matcher.match(descriptors1, descriptors2, matches);

	std::cout << "Number of matched points: " << matches.size() << std::endl;

	nth_element(matches.begin(), matches.begin() + 24, matches.end());
	matches.erase(matches.begin() + 25, matches.end());
	Mat imaMatches;
	drawMatches(image3, keypoints3, image4, keypoints4, matches, imaMatches, Scalar::all(-1));
	imshow("matcher", imaMatches);



	waitKey(0);
}