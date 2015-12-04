#ifndef MATCH_H
#define MATCH_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/nonfree/nonfree.hpp>
using namespace std;
using namespace cv;
class RobusMatcher
{
public:
	RobusMatcher() :ratio(0.65f), reflineF(true), confidence(0.99), distance(3.0){
		detector = new SurfFeatureDetector();
		extractor = new SurfDescriptorExtractor();
	}

	// Set the feature detector
	void setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect) {

		detector = detect;
	}


	void setDescriptorExtractor(Ptr<SurfDescriptorExtractor>&desc){
		extractor = desc;
	}
	// Set the minimum distance to epipolar in RANSAC
	void setMinDistanceToEpipolar(double d) {

		distance = d;
	}

	// Set confidence level in RANSAC
	void setConfidenceLevel(double c) {

		confidence = c;
	}

	// Set the NN ratio
	void setRatio(float r) {

		ratio = r;
	}

	// if you want the F matrix to be recalculated
	void refineFundamental(bool flag) {

		reflineF = flag;
	}
	Mat match(Mat &img1, Mat &img2, vector<DMatch>&matches, vector<KeyPoint> &keypoints1, vector<KeyPoint>&keypoints2){
		detector->detect(img1, keypoints1);
		detector->detect(img2, keypoints2);
		Mat descriptors1, descriptors2;
		extractor->compute(img1, keypoints1, descriptors1);
		extractor->compute(img2, keypoints2, descriptors2);
		BruteForceMatcher<L2<float>> matcher;
		vector<vector<DMatch>> matches1;
		matcher.knnMatch(descriptors1, descriptors2, matches1, 2);
		vector<vector<DMatch>> matches2;
		matcher.knnMatch(descriptors1, descriptors2, matches2, 2);
		
		// 3. Remove matches for which NN ratio is > than threshold

		// clean image 1 -> image 2 matches
		int removed = ratioTest(matches1);
		std::cout << "Number of matched points 1->2 (ratio test) : " << matches1.size() - removed << std::endl;
		// clean image 2 -> image 1 matches
		removed = ratioTest(matches2);
		std::cout << "Number of matched points 1->2 (ratio test) : " << matches2.size() - removed << std::endl;

		// 4. Remove non-symmetrical matches
		std::vector<cv::DMatch> symMatches;
		symmetryTest(matches1, matches2, symMatches);

		std::cout << "Number of matched points (symmetry test): " << symMatches.size() << std::endl;

		// 5. Validate matches using RANSAC
		cv::Mat fundemental = ransacTest(symMatches, keypoints1, keypoints2, matches);

		// return the found fundemental matrix
		return fundemental;
	}

	// Clear matches for which NN ratio is > than threshold
	// return the number of removed points 
	// (corresponding entries being cleared, i.e. size will be 0)
	int ratioTest(std::vector<std::vector<cv::DMatch>>& matches) {

		int removed = 0;

		// for all matches
		for (std::vector<std::vector<cv::DMatch>>::iterator matchIterator = matches.begin();
			matchIterator != matches.end(); ++matchIterator) {

			// if 2 NN has been identified
			if (matchIterator->size() > 1) {

				// check distance ratio
				if ((*matchIterator)[0].distance / (*matchIterator)[1].distance > ratio) {

					matchIterator->clear(); // remove match
					removed++;
				}

			}
			else { // does not have 2 neighbours

				matchIterator->clear(); // remove match
				removed++;
			}
		}

		return removed;
	}


	// Insert symmetrical matches in symMatches vector
	void symmetryTest(const std::vector<std::vector<cv::DMatch>>& matches1,
		const std::vector<std::vector<cv::DMatch>>& matches2,
		std::vector<cv::DMatch>& symMatches) {

		// for all matches image 1 -> image 2
		for (std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator1 = matches1.begin();
			matchIterator1 != matches1.end(); ++matchIterator1) {

			if (matchIterator1->size() < 2) // ignore deleted matches 
				continue;

			// for all matches image 2 -> image 1
			for (std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator2 = matches2.begin();
				matchIterator2 != matches2.end(); ++matchIterator2) {

				if (matchIterator2->size() < 2) // ignore deleted matches 
					continue;

				// Match symmetry test
				if ((*matchIterator1)[0].queryIdx == (*matchIterator2)[0].trainIdx &&
					(*matchIterator2)[0].queryIdx == (*matchIterator1)[0].trainIdx) {

					// add symmetrical match
					symMatches.push_back(cv::DMatch((*matchIterator1)[0].queryIdx,
						(*matchIterator1)[0].trainIdx,
						(*matchIterator1)[0].distance));
					break; // next match in image 1 -> image 2
				}
			}
		}
	}

	// Identify good matches using RANSAC
	// Return fundemental matrix
	cv::Mat ransacTest(const std::vector<cv::DMatch>& matches,
		const std::vector<cv::KeyPoint>& keypoints1,
		const std::vector<cv::KeyPoint>& keypoints2,
		std::vector<cv::DMatch>& outMatches) {

		// Convert keypoints into Point2f	
		std::vector<cv::Point2f> points1, points2;
		for (std::vector<cv::DMatch>::const_iterator it = matches.begin();
			it != matches.end(); ++it) {

			// Get the position of left keypoints
			float x = keypoints1[it->queryIdx].pt.x;
			float y = keypoints1[it->queryIdx].pt.y;
			points1.push_back(cv::Point2f(x, y));
			// Get the position of right keypoints
			x = keypoints2[it->trainIdx].pt.x;
			y = keypoints2[it->trainIdx].pt.y;
			points2.push_back(cv::Point2f(x, y));
		}

		// Compute F matrix using RANSAC
		std::vector<uchar> inliers(points1.size(), 0);
		cv::Mat fundemental = cv::findFundamentalMat(
			cv::Mat(points1), cv::Mat(points2), // matching points
			inliers,      // match status (inlier ou outlier)  
			CV_FM_RANSAC, // RANSAC method
			distance,     // distance to epipolar line
			confidence);  // confidence probability

		// extract the surviving (inliers) matches
		std::vector<uchar>::const_iterator itIn = inliers.begin();
		std::vector<cv::DMatch>::const_iterator itM = matches.begin();
		// for all matches
		for (; itIn != inliers.end(); ++itIn, ++itM) {

			if (*itIn) { // it is a valid match

				outMatches.push_back(*itM);
			}
		}

		std::cout << "Number of matched points (after cleaning): " << outMatches.size() << std::endl;

		if (reflineF) {
			// The F matrix will be recomputed with all accepted matches

			// Convert keypoints into Point2f for final F computation	
			points1.clear();
			points2.clear();

			for (std::vector<cv::DMatch>::const_iterator it = outMatches.begin();
				it != outMatches.end(); ++it) {

				// Get the position of left keypoints
				float x = keypoints1[it->queryIdx].pt.x;
				float y = keypoints1[it->queryIdx].pt.y;
				points1.push_back(cv::Point2f(x, y));
				// Get the position of right keypoints
				x = keypoints2[it->trainIdx].pt.x;
				y = keypoints2[it->trainIdx].pt.y;
				points2.push_back(cv::Point2f(x, y));
			}

			// Compute 8-point F from all accepted matches
			fundemental = cv::findFundamentalMat(
				cv::Mat(points1), cv::Mat(points2), // matching points
				CV_FM_8POINT); // 8-point method
		}

		return fundemental;
	}


private:
	Ptr<FeatureDetector> detector;
	Ptr<DescriptorExtractor> extractor;

	float ratio;
	bool reflineF;
	double distance;
	double confidence;

};



#endif