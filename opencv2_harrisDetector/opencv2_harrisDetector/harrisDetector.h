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
	//����Harris�ǵ�
	void detect(const Mat &img){
		cornerHarris(img, cornerStrength, neighbourhood, aperture, k);

	//�ڲ���ֵ����
	double minStrength;//δʹ��
	minMaxLoc(cornerStrength, &minStrength, &maxStrength);//�ֲ����ֵ���
	Mat dilated;//��ʱͼ��
	dilate(cornerStrength, dilated, Mat());
	compare(cornerStrength, dilated, localmax, CMP_EQ);
	}
private:
	Mat cornerStrength; //��ʾ�ǵ�ǿ�ȵ�32λ����ͼ��
	Mat cornerTh;		//��ʾ��ֵ��֮���32λ����ͼ��
	Mat localmax;		//�ֲ�����ֵͼ��
	int neighbourhood;	//����ƽ�����������صĳߴ�
	int aperture;		//�ݶȼ���Ŀ׾���С
	double k;			//Harris����
	double maxStrength; //��ֵ��������ǿ��
	double threshold1; //����õ�����ֵ
	int nonMaxSize;		//�Ǽ���ֵ���Ƶ��������صĳߴ�
	Mat kernel;			//�Ǽ���ֵ���Ƶĺ�
};

harrisDetector::harrisDetector() :neighbourhood(3), aperture(3), k(0.01), threshold1(0.01), nonMaxSize(3)
{
	setLocalMaxWindowSize(nonMaxSize);
}

harrisDetector::~harrisDetector()
{
}
#endif