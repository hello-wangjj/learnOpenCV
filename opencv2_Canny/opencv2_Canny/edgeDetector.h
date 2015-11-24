#ifndef EDGEDETECTOR_H
#define EDGEDETECTOR_H
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
class EdgeDetector
{
public:
	EdgeDetector();
	~EdgeDetector();

private:
	Mat img;

};

EdgeDetector::EdgeDetector()
{
}

EdgeDetector::~EdgeDetector()
{
}


#endif