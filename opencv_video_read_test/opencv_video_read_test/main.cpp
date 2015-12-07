#include <iostream>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;
#define WINDOW_SIZE "¹ö¶¯Ìõ"
int g_sliderposition;
int g_position;
VideoCapture capture;
Mat frame;
void on_Trackbar(int, void*)
{
	capture.set(CV_CAP_PROP_POS_FRAMES, g_position);
	//imshow(WINDOW_SIZE, frame);
}
int main()
{
	VideoCapture capture("bike.avi");
	namedWindow(WINDOW_SIZE, WINDOW_NORMAL);
	//Mat frame;
	//capture >> frame;
	//imshow(WINDOW_SIZE,frame);
	g_sliderposition = capture.get(CV_CAP_PROP_FRAME_COUNT);
	g_position = capture.get(CV_CAP_PROP_POS_FRAMES);
	char TrackbarName[50];
	sprintf_s(TrackbarName, "Ö¡Êý£º%d", g_sliderposition);
	createTrackbar(TrackbarName, WINDOW_SIZE, &g_position, g_sliderposition, on_Trackbar);
	//on_Trackbar(g_position,0);
	//int n = 0;
	while (1){
		//Mat frame;
		capture >> frame;
		imshow(WINDOW_SIZE, frame);
		//n = capture.get(CV_CAP_PROP_POS_FRAMES);
		//setTrackbarPos(TrackbarName, WINDOW_SIZE, n);
		on_Trackbar(g_position, 0);
		char c = waitKey(33);
		if (c == 27) break;
	}
}
