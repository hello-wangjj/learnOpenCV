#ifndef LAPLACIAN_H
#define LAPLACIAN_H
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
class LaplacianZC
{
public:
	LaplacianZC() :aperture(3){}
	void setAperture(int a){
			aperture = a;
		}
	Mat computeLaplacian(const Mat& image){
			Laplacian(image, laplace, CV_32F, aperture);
			img = image.clone();
			return laplace;
		}
	Mat getLaplacianImage(double scale = -1.0){
		if (scale < 0){
			double lapmin, lapmax;
			minMaxLoc(laplace, &lapmin, &lapmax);
			scale = 127 / max(-lapmin, lapmax);
		}
		Mat laplaceImage;
		laplace.convertTo(laplaceImage, CV_8U, scale, 128);
		return laplaceImage;
	}

	// Get a binary image of the zero - crossings
	// if the product of the two adjascent pixels is
	// less than threshold then this zero-crossing will be ignored
	Mat getZeroCrossings(float threshold = 0){
		Mat_<float>::const_iterator it = laplace.begin<float>()+laplace.step1();
		Mat_<float>::const_iterator itend = laplace.end<float>();
		Mat_<float>::const_iterator itup = laplace.begin<float>();
		// Binary image initialize to white
		Mat binary(laplace.size(), CV_8U, Scalar(255));
		Mat_<uchar>::iterator itout = binary.begin<uchar>() + binary.step1();
		// negate the input threshold value
		threshold *= -1;
		for (; it != itend; ++it, ++itup, ++itout){
			// if the product of two adjascent pixel is negative
			// then there is a sign change
			if (*it * *(it - 1) < threshold) *itout = 0;
			else if (*it * *itup<threshold)
			{
				*itout = 0;
			}
		}
		return binary;
	}

	// Get a binary image of the zero-crossings
	// if the product of the two adjacent pixels is
	// less than threshold then this zero-crossing will be ignored
	cv::Mat getZeroCrossingsWithSobel(float threshold) {

		cv::Mat sx;
		cv::Sobel(img, sx, CV_32F, 1, 0, 1);
		cv::Mat sy;
		cv::Sobel(img, sy, CV_32F, 0, 1, 1);

		// Create the iterators
		cv::Mat_<float>::const_iterator it = laplace.begin<float>() + laplace.step1();
		cv::Mat_<float>::const_iterator itend = laplace.end<float>();
		cv::Mat_<float>::const_iterator itup = laplace.begin<float>();
		cv::Mat_<float>::const_iterator itx = sx.begin<float>() + sx.step1();
		cv::Mat_<float>::const_iterator ity = sy.begin<float>() + sy.step1();

		// Binary image initialize to white
		cv::Mat binary(laplace.size(), CV_8U, cv::Scalar(255));
		cv::Mat_<uchar>::iterator itout = binary.begin<uchar>() + binary.step1();

		for (; it != itend; ++it, ++itup, ++itout, ++itx, ++ity) {

			// if the product of two adjacent pixel is negative
			// then there is a sign change
			if (*it * *(it - 1) < 0.0 && fabs(*ity) > threshold)
				*itout = 0; // horizontal zero-crossing
			else if (*it * *itup < 0.0 && fabs(*ity) > threshold)
				*itout = 0; // vertical zero-crossing
		}

		return binary;
	}

	//~Laplacian();

private:
	Mat img;
	Mat laplace;
	int aperture;
};

/*Laplacian::Laplacian()
{
}

Laplacian::~Laplacian()
{
}*/



#endif