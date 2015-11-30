#ifndef HARRISDETECTOR_H
#define HARRISDETECTOR_H

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class harrisDetector
{
public:
	harrisDetector();
	~harrisDetector();
	void setLocalMaxWindowSize(int size){
		nonMaxSize = size;
		kernel.create(nonMaxSize, nonMaxSize, CV_8U);
	}
	//计算Harris角点
	void detect(const Mat &img){
		cornerHarris(img, cornerStrength, neighbourhood, aperture, k);

	//内部阈值计算
	double minStrength;//未使用
	minMaxLoc(cornerStrength, &minStrength, &maxStrength);//局部最大值检测
	Mat dilated;//临时图像
	dilate(cornerStrength, dilated, Mat());
	compare(cornerStrength, dilated, localmax, CMP_EQ);
	}
	Mat getCornerMap(double qualitylevel){
		Mat cornerMap;
		threshold1 = qualitylevel*maxStrength;
		threshold(cornerStrength, cornerTh, threshold1, 255, THRESH_BINARY);
		cornerTh.convertTo(cornerMap, CV_8U);
		bitwise_and(cornerMap, localmax,cornerMap);
		return cornerMap;
	}
	void getCorners(vector<Point> &points, double qualityLevel){
		Mat cornerMap = getCornerMap(qualityLevel);
		getCorners(points, cornerMap);
	}
	void getCorners(vector<Point>&points, Mat &cornerMap){
		for (int y = 0; y < cornerMap.rows; y++){
			const uchar*rowPtr = cornerMap.ptr<uchar>(y);
			for (int x = 0; x < cornerMap.cols; x++){
				if (rowPtr[x]){
					points.push_back(Point(x, y));
				}
			}
		}
	}
	void drawOnImg(Mat &img, vector<Point>&poins, Scalar color = Scalar::all(255), int radius = 3, int thickness = 2){
		vector<Point>::const_iterator itp = poins.begin();
		while (itp != poins.end()){
			circle(img, *itp, radius, color, thickness);
			++itp;
		}
	}
private:
	Mat cornerStrength; //表示角点强度的32位浮点图像
	Mat cornerTh;		//表示阈值化之后的32位浮点图像
	Mat localmax;		//局部极大值图像
	int neighbourhood;	//导数平滑的相邻像素的尺寸
	int aperture;		//梯度计算的孔径大小
	double k;			//Harris参数
	double maxStrength; //阈值计算的最大强度
	double threshold1; //计算得到的阈值
	int nonMaxSize;		//非极大值抑制的相邻像素的尺寸
	Mat kernel;			//非极大值抑制的核
};

harrisDetector::harrisDetector() :neighbourhood(3), aperture(3), k(0.01), threshold1(0.01), nonMaxSize(3)
{
	setLocalMaxWindowSize(nonMaxSize);
}

harrisDetector::~harrisDetector()
{
}
#endif