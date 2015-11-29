#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;
int main()
{
	Mat image = imread("binaryGroup.bmp",0);
	namedWindow("binary img", WINDOW_AUTOSIZE);
	imshow("binary img", image);

	vector<vector<Point>> contours;
	findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	std::cout << "Contours: " << contours.size() << std::endl;
	vector<vector<Point>>::const_iterator itc = contours.begin();
	for (; itc != contours.end(); itc++){
		cout << "size:" << itc->size() << " ";
	}
	cout << endl;

	Mat result(image.size(), CV_8U, Scalar(255));


	drawContours(result, contours, -1, Scalar(0), 2);
	namedWindow("result img", WINDOW_AUTOSIZE);
	imshow("result img", result);

	int minC = 100;
	int maxC = 1000;
	vector<vector<Point>>::const_iterator itc2 = contours.begin();
	while (itc2!=contours.end())
	{
		if (itc2->size()<minC || itc2->size()>maxC) itc2 = contours.erase(itc2);
		else ++itc2;
	}
	std::cout << "Contours: " << contours.size() << std::endl;
	Mat result2(image.size(), CV_8U, Scalar(255));
	drawContours(result2, contours, -1, Scalar(0), 2);
	Mat src0 = imread("group.jpg");


	drawContours(src0, contours, -1, Scalar(255), 2);
	imshow("src0", src0);
	namedWindow("result2 img", WINDOW_AUTOSIZE);
	imshow("result2 img", result2);
	Mat dst;
	Mat src = imread("group.jpg",0);

	addWeighted(src, 0.7, result2,0.3, 0, dst);
	imshow("dts img", dst);



	Mat src2 = imread("group.jpg");
	drawContours(src2, contours, -1, Scalar::all(255), 2);
	imshow("Contours on Animals", src2);
	//外轮廓

	vector<vector<Point>> contours_list;
	findContours(image, contours_list, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	Mat result_list(image.size(), CV_8U, Scalar(255));


	drawContours(result_list, contours_list, -1, Scalar(0), 2);
	namedWindow("result_list img", WINDOW_AUTOSIZE);
	imshow("result_list img", result_list);


	Rect r0 = boundingRect(Mat(contours[0]));
	rectangle(result2, r0, Scalar(0), 2);
	//测试最小包围圆
	float radius;
	Point2f center;
	minEnclosingCircle(Mat(contours[1]), center, radius);
	circle(result2, Point(center), static_cast<int>(radius), Scalar(0), 2);
	//测试多边形近似
	vector<Point>poly;
	approxPolyDP(Mat(contours[2]), poly, 5, true);
	vector<Point>::const_iterator itp = poly.begin();
	while (itp!=(poly.end()-1))
	{
		line(result2, *itp, *(itp + 1), Scalar(0), 2);
		++itp;
	}
	line(result2, *(poly.begin()), *(poly.end() - 1), Scalar(0), 2);

	//测试凸包
	vector<Point>hull;
	convexHull(Mat(contours[3]), hull);
	//测试力矩，遍历所有轮廓
	//vector<Point>::const_iterator itc = contours.begin(); 定义过
	itc = contours.begin();
	while (itc != contours.end()){
		Moments mom = moments(Mat(*itc++));
		circle(result2, Point(mom.m10 / mom.m00, mom.m01 / mom.m00), 2, Scalar(0), 2);
	}
	imshow("contours", result2);

	waitKey(0);
}