
#include <opencv2/opencv.hpp>

#include "watershed.h"
using namespace cv;
int main()
{
	Mat img1 = imread("group.jpg");
	namedWindow("original", WINDOW_AUTOSIZE);
	imshow("original", img1);

	//get the binary img
	Mat binary = imread("binary.bmp",0);
	imshow("binary", binary);

	// Eliminate noise and smaller objects
	Mat fg;
	erode(binary, fg, cv::Mat(), Point(-1, -1), 6);

	// Display the foreground image
	namedWindow("Foreground Image");
	imshow("Foreground Image", fg);

	// Identify image pixels without objects
	Mat bg;
	dilate(binary, bg, cv::Mat(), cv::Point(-1, -1), 6);
	threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);

	// Display the background image
	namedWindow("Background Image");
	imshow("Background Image", bg);

	// Show markers image
	Mat markers(binary.size(), CV_8U, cv::Scalar(0));
	markers = fg + bg;
	namedWindow("Markers");
	imshow("Markers", markers);

	// Create watershed segmentation object
	watersheds segmenter;

	// Set markers and process
	segmenter.setmarkers(markers);
	segmenter.process(img1);

	// Display segmentation result
	namedWindow("Segmentation");
	imshow("Segmentation", segmenter.getSegmentation());

	// Display watersheds
	cv::namedWindow("Watersheds");
	cv::imshow("Watersheds", segmenter.getWatersheds());


	// Open another image
	Mat  image = imread("tower.jpg");

	// Identify background pixels
	cv::Mat imageMask(image.size(), CV_8U, cv::Scalar(0));
	cv::rectangle(imageMask, cv::Point(5, 5), cv::Point(image.cols - 5, image.rows - 5), cv::Scalar(255), 3);
	// Identify foreground pixels (in the middle of the image)
	cv::rectangle(imageMask, cv::Point(image.cols / 2 - 10, image.rows / 2 - 10),
		cv::Point(image.cols / 2 + 10, image.rows / 2 + 10), cv::Scalar(1), 10);

	// Set markers and process
	segmenter.setmarkers(imageMask);
	segmenter.process(image);

	// Display the image with markers
	cv::rectangle(image, cv::Point(5, 5), cv::Point(image.cols - 5, image.rows - 5), cv::Scalar(255, 255, 255), 3);
	cv::rectangle(image, cv::Point(image.cols / 2 - 10, image.rows / 2 - 10),
		cv::Point(image.cols / 2 + 10, image.rows / 2 + 10), cv::Scalar(1, 1, 1), 10);
	cv::namedWindow("Image with marker");
	cv::imshow("Image with marker", image);

	// Display watersheds
	cv::namedWindow("Watersheds of foreground object");
	cv::imshow("Watersheds of foreground object", segmenter.getWatersheds());

	// Open another image
	image = cv::imread("tower.jpg");

	// define bounding rectangle 
	cv::Rect rectangle(50, 70, image.cols - 150, image.rows - 180);

	cv::Mat result; // segmentation result (4 possible values)
	cv::Mat bgModel, fgModel; // the models (internally used)
	// GrabCut segmentation
	cv::grabCut(image,    // input image
		result,   // segmentation result
		rectangle,// rectangle containing foreground 
		bgModel, fgModel, // models
		1,        // number of iterations
		cv::GC_INIT_WITH_RECT); // use rectangle

	// Get the pixels marked as likely foreground
	cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
	// Generate output image
	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	image.copyTo(foreground, result); // bg pixels not copied

	// draw rectangle on original image
	cv::rectangle(image, rectangle, cv::Scalar(255, 255, 255), 1);
	cv::namedWindow("Image");
	cv::imshow("Image", image);

	// display result
	cv::namedWindow("Segmented Image");
	cv::imshow("Segmented Image", foreground);

	// Open another image
	image = cv::imread("group.jpg");

	// define bounding rectangle 
	cv::Rect rectangle2(10, 100, 380, 180);

	cv::Mat bkgModel, fgrModel; // the models (internally used)
	// GrabCut segmentation
	cv::grabCut(image,  // input image
		result, // segmentation result
		rectangle2, bkgModel, fgrModel, 5, cv::GC_INIT_WITH_RECT);
	// Get the pixels marked as likely foreground
	//	cv::compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
	result = result & 1;
	foreground.create(image.size(), CV_8UC3);
	foreground.setTo(cv::Scalar(255, 255, 255));
	image.copyTo(foreground, result); // bg pixels not copied

	// draw rectangle on original image
	cv::rectangle(image, rectangle2, cv::Scalar(255, 255, 255), 1);
	cv::namedWindow("Image 2");
	cv::imshow("Image 2", image);

	// display result
	cv::namedWindow("Foreground objects");
	cv::imshow("Foreground objects", foreground);

	waitKey(0);
}