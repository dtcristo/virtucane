#include "read_text.h"

#include "opencv2/opencv.hpp"
#include <iostream>
#include <ctime>
#include <sys/time.h>

using namespace std;
using namespace cv;

// Program defaults
int imageMode_ = IMAGE_MODE_BINARY;
bool verbose_ = false;
bool scoring_ = true;
bool speak_ = true;
int speakMax_ = 0;

int
main(int argc, char* argv[])
{
    namedWindow("Video", 1);
    namedWindow("Detection", 1);

    VideoCapture cap = VideoCapture(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded
        return -1;

    int framecount = 0;
    timeval start;
    gettimeofday(&start, 0);
    for (;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera

        imshow("Video", frame);

        int c = waitKey(1);
        if ((char) c == 27 /*ESC key*/)
        {
            timeval end;
            gettimeofday(&end, 0);
            int seconds = (end.tv_sec - start.tv_sec);
            int fps = framecount / seconds + 0.5;
            cout << "fps = " << fps << endl;
            break;
        }
        else if ((char) c == ' ')
        {
            imwrite("frame.jpg", frame);
            cout << "Frame saved" << endl;

            DetectText detector = DetectText();
            detector.detect("frame.jpg");
        }
        framecount++;
    }
    return 0;
}
