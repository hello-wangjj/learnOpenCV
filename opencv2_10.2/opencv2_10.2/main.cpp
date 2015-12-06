#include <iostream>
#include <opencv2/opencv.hpp>
#include "videoProcessor.h"
using namespace std;
using namespace cv;
void canny(Mat &img, Mat &out){
	if (img.channels() == 3) cvtColor(img, img, COLOR_BGR2GRAY);
	Canny(img, out, 100, 200);
	threshold(out, out, 128, 255, THRESH_BINARY_INV);
}
int main()
{
	VideoCapture capture("bike.avi");
	if (!capture.isOpened()) return 1;
	double rate = capture.get(CV_CAP_PROP_FPS);
	bool stop(false);
	Mat frame;
	namedWindow("Extractor Frame");
	int delay = 1000 / rate;
	while (!stop)
	{
		if (!capture.read(frame)) break;
		imshow("Extractor Frame", frame);
		//ÒýÈëÑÓ³Ù
		if (waitKey(delay) >= 0) stop = true;
	}
	capture.release();

	VideoProcessor process;
	process.setIput("bike.avi");
	process.displayInput("Input Video");
	process.displayOutput("Output Video");
	// Play the video at the original frame rate
	process.setDelay(1000. / process.getFrameRate());
	// Set the frame processor callback function
	process.setFrameprocessor(canny);
	process.run();
	waitKey();



}