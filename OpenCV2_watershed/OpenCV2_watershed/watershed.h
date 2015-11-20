#ifndef WATERSHED_H
#define WATERSHED_H
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
class watersheds
{
public:
	//watersheds();
	//~watersheds();
	void setmarkers(const Mat &markeImg)
	{
		markeImg.convertTo(markers, CV_32S);
	}
	Mat process(const Mat &Img)
	{
		watershed(Img, markers);
		return markers;
	}
	// Return result in the form of an image
	cv::Mat getSegmentation() {
		cv::Mat tmp;
		// all segment with label higher than 255
		// will be assigned value 255
		markers.convertTo(tmp, CV_8U);

		return tmp;
	}

	// Return watershed in the form of an image
	cv::Mat getWatersheds() {

		cv::Mat tmp;
		markers.convertTo(tmp, CV_8U, 255, 255);

		return tmp;
	}
private:
	Mat markers;

};

/*watersheds::watersheds()
{
}

watersheds::~watersheds()
{
}*/



#endif
