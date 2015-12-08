#include "featuretrack.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
int main()
{
	// Create video procesor instance
	VideoProcessor1 processor;

	// Create feature tracker instance
	FeatureTracker tracker;

	// Open video file
	processor.setInput("bike.avi");

	// set frame processor
	processor.setFrameProcessor(&tracker);

	// Declare a window to display the video
	processor.displayOutput("Tracked Features");

	// Play the video at the original frame rate
	processor.setDelay(1000. / processor.getFrameRate());

	// Start the process
	processor.run();

	cv::waitKey();
	/*VideoProcessor1 processor;
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
	processor.setOutput("../output/bike.avi", processor.getCodec(codec), processor.getFrameRate());
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

	cv::waitKey();*/
}