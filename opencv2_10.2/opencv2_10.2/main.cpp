#include <iostream>
#include <opencv2/opencv.hpp>
#include "videoProcessor.h"
#include "frameprocessor.h"
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
		//if (!capture.read(frame)) break;
		capture >> frame;
		imshow("Extractor Frame", frame);
		//ÒýÈëÑÓ³Ù
		if (waitKey(delay) >= 0) stop = true;
	
	}
	capture.release();
	//test
	VideoCapture capture1;
	Mat frame1;
	capture1.open("bike.avi");
	namedWindow("test");
	while (!stop)
	{
		capture1 >> frame1;
		imshow("test", frame1);
		if (waitKey(delay) >= 0) stop = true;
	}
	int n = capture1.get(CV_CAP_PROP_FRAME_COUNT);
	cout << n << endl;
	capture1.release();
	//second test
	VideoProcessor process;
	process.setInput("bike.avi");
	process.displayInput("Input Video");
	process.displayOutput("Output Video");
	// Play the video at the original frame rate
	process.setDelay(1000. / process.getFrameRate());
	// Set the frame processor callback function
	process.setFrameprocessor(canny);
	process.run();
	waitKey();

	// Second test
	// Create instance
	//	VideoProcessor processor;
	// Open video file
	VideoProcessor1 processor;
	processor.setInput("bike.avi");
	// Get basic info about video file
	cv::Size size = processor.getFrameSize();
	std::cout << size.width << " " << size.height << std::endl;
	std::cout << processor.getFrameRate() << std::endl;
	std::cout << processor.getTotalFrameCount() << std::endl;
	std::cout << processor.getFrameNumber() << std::endl;
	std::cout << processor.getPositionMS() << std::endl;

	// No processing
	processor.dontCallProcess();
	// Output filename
	//	processor.setOutput("../output/bikeOut",".jpg");
	char codec[4];
	processor.setOutput("bike2.avi", processor.getCodec(codec), processor.getFrameRate());
	std::cout << "Codec: " << codec[0] << codec[1] << codec[2] << codec[3] << std::endl;

	// Position the stream at frame 300
	//	processor.setFrameNumber(300);
	//	processor.stopAtFrameNo(120);

	// Declare a window to display the video
	processor.displayInput("Current Frame");
	processor.displayOutput("Output Frame");

	// Play the video at the original frame rate
	processor.setDelay(1000. / processor.getFrameRate());

	// Start the process
	processor.run();

	std::cout << processor.getFrameNumber() << std::endl;
	std::cout << processor.getPositionMS() << std::endl;

	cv::waitKey();




}