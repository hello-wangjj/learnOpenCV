#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
class VideoProcessor
{
public:
	VideoProcessor() :callIt(true), delay(0), fnumber(0), stop(false), frameToStop(-1){}
	~VideoProcessor(){}
	void setFrameprocessor(
		void(*fameProssingCallback)(Mat &, Mat &)){
		process = fameProssingCallback;
	}
	bool setIput(string fileName){
		fnumber = 0;
		capture.release();
		return capture.open(fileName);
	}
	void displayInput(string wn){
		windowNameInput = wn;
		namedWindow(windowNameInput);
	}
	void displayOutput(string wn){
		windowNameOutput = wn;
		namedWindow(windowNameOutput);
	}
	void dontDisplay(){
		destroyWindow(windowNameInput);
		destroyWindow(windowNameOutput);
		windowNameInput.clear();
		windowNameOutput.clear();
	}
	void run(){
		Mat frame;//当前帧
		Mat output;
		if (!isOpened()) return;
		stop = false;
		while (!isStopped()){
			if (!readNextFrame(frame)) break;
			if (windowNameInput.length() != 0)
				imshow(windowNameInput, frame);
			if (callIt){
				process(frame, output);
				fnumber++;
			}
			else{
				output = frame;
			}
			if (windowNameOutput.length() != 0)
				imshow(windowNameOutput, output);
			if (delay > 0 && waitKey(delay) >= 0)
				stopIt();
			if (frameToStop >= 0 && getFrameNumber() == frameToStop)
				stopIt();
		}
	}
	void stopIt(){
		stop = true;
	}
	bool isStopped(){
		return stop;
	}
	bool isOpened(){
		//capture.isOpened();
		return capture.isOpened() || !images.empty();
	}
	void setDelay(int d){
		delay = d;
	}
	bool readNextFrame(Mat &frame){
		return capture.read(frame);
	}
	void callProcess(){
		callIt = true;
	}
	void dontCallProcess(){
		callIt = false;
	}
	void stopAtFrameNo(long frame){
		frameToStop = frame;
	}
	long getFrameNumber(){
		long fnumber = static_cast<long>(capture.get(CV_CAP_PROP_FRAME_COUNT));
		return fnumber;
	}


	// return the frame rate
	double getFrameRate() {

		// undefined for vector of images
		if (images.size() != 0) return 0;

		double r = capture.get(CV_CAP_PROP_FPS);
		return r;
	}


private:
	VideoCapture capture;
	void(*process)(Mat &, Mat&);//每帧调用的回调函数
	bool callIt;
	string windowNameInput;
	string windowNameOutput;
	int delay;
	long fnumber;//已处理帧数
	long frameToStop;//停止帧数
	bool stop;

	// vector of image filename to be used as input
	vector<string> images;
	// image vector iterator
	vector<string>::const_iterator itImg;
};


