#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main()
{
	Mat image = imread("boldt.jpg");
	if (!image.data) return 0;
	namedWindow("origin img", WINDOW_AUTOSIZE);
	imshow("origin img", image);

	Mat img_huidu;
	cvtColor(image, img_huidu, COLOR_BGR2GRAY);
	namedWindow("huidu Img", WINDOW_AUTOSIZE);
	imshow("huidu Img", img_huidu);


	Mat result;
	GaussianBlur(img_huidu, result, Size(5, 5), 1.5);
	namedWindow("Gaussian Img", WINDOW_AUTOSIZE);
	imshow("Gaussian Img", result);

	// Get the gaussian kernel (1.5)
	Mat gauss = getGaussianKernel(9, 1.5, CV_32F);

	// Display kernel values
	Mat_<float>::const_iterator it = gauss.begin<float>();
	Mat_<float>::const_iterator itend = gauss.end<float>();
	cout << "[";
	for (; it != itend; ++it) {
		cout << *it << " ";
	}
	cout << "]" <<endl;

	// Get the gaussian kernel (0.5)
	gauss = getGaussianKernel(9, 0.5, CV_32F);


	// Display kernel values
	it = gauss.begin<float>();
	itend = gauss.end<float>();
	std::cout << "[";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// Get the gaussian kernel (2.5)
	gauss = cv::getGaussianKernel(9, 2.5, CV_32F);

	// Display kernel values
	it = gauss.begin<float>();
	itend = gauss.end<float>();
	std::cout << "[";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// Get the Deriv kernel (2.5)
	cv::Mat kx, ky;
	cv::getDerivKernels(kx, ky, 2, 2, 7, true);

	// Display kernel values
	cv::Mat_<float>::const_iterator kit = kx.begin<float>();
	cv::Mat_<float>::const_iterator kitend = kx.end<float>();
	std::cout << "[";
	for (; kit != kitend; ++kit) {
		std::cout << *kit << " ";
	}
	std::cout << "]" << std::endl;

	Mat resizeImg;
	resize(image, resizeImg, Size(image.cols / 3, image.rows / 3));
	namedWindow("resize Img", WINDOW_AUTOSIZE);
	imshow("resize Img", resizeImg);

	// Blur the image with a mean filter
	cv::blur(image, result, cv::Size(5, 5));
	// Display the blurred image
	cv::namedWindow("Mean filtered Image");
	cv::imshow("Mean filtered Image", result);


	waitKey(0);
}