#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <sys/time.h>

using namespace std;
using namespace cv;

int main();
void initGpio();
void setGpio(int pin);
void resetGpio(int pin);

int main()
{
    initGpio();

    VideoCapture cap = VideoCapture(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded
    {
        cout << "Failed to open webcam" << endl;
        return -1;
    }

    Mat frame;
    int framecount = 0;
    timeval start;
    gettimeofday(&start, 0);

    for (;;)
    {
        cap >> frame;

        // Create mask
        Mat frameHsv;
        cvtColor(frame, frameHsv, CV_BGR2HSV);
        Mat frameThresh;
        inRange(frameHsv, Scalar(100, 75, 75), Scalar(140, 255, 255), frameThresh);


        Moments myMoments = moments(frameThresh, true);

        int x = myMoments.m10/myMoments.m00;
        int y = myMoments.m01/myMoments.m00;

        if (countNonZero(frameThresh) > 2000)
        {
            //LEFT
            if (x<213)
            {
                //TOP
                if (y<160)
                {
                    cout << "TOP LEFT" << endl;
                    setGpio(1);
                }
                //BOTTOM
                else if (y>320)
                {
                    cout << "BOTTOM LEFT" << endl;
                    setGpio(3);
                }
                //MID
                else
                {
                    cout << "MID LEFT" << endl;
                    setGpio(2);
                }
            }
            //RIGHT
            else if (x>427)
            {
                //TOP
                if (y<160)
                {
                    cout << "TOP RIGHT" << endl;
                    setGpio(7);
                }
                //BOTTOM
                else if (y>320)
                {
                    cout << "BOTTOM RIGHT" << endl;
                    setGpio(9);
                }
                //MID
                else
                {
                    cout << "MID RIGHT" << endl;
                    setGpio(8);
                }
            }
            //CENTRE
            else
            {
                //TOP
                if (y<160)
                {
                    cout << "TOP CENTRE" << endl;
                    setGpio(4);
                }
                //BOTTOM
                else if (y>320)
                {
                    cout << "BOTTOM CENTRE" << endl;
                    setGpio(6);
                }
                //MID
                else
                {
                    cout << "MID CENTRE" << endl;
                    setGpio(5);
                }
            }
        }
        else
        {
            //NONE
            resetGpio(0);
        }

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
        }
        framecount++;
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

void initGpio(){
    // Setup special pins
    system("devmem2 0x480021c8 h 0x10c");
    system("devmem2 0x480021ca h 0x10c");
    system("devmem2 0x480021cc h 0x10c");
    system("devmem2 0x480021ce h 0x10c");
    system("devmem2 0x480021d0 h 0x10c");
    cout << "done: special setup" << endl;

    // Export pins
    system("echo 171 > /sys/class/gpio/export");
    system("echo 172 > /sys/class/gpio/export");
    system("echo 174 > /sys/class/gpio/export");
    system("echo 173 > /sys/class/gpio/export");
    system("echo 175 > /sys/class/gpio/export");
    system("echo 146 > /sys/class/gpio/export");
    system("echo 145 > /sys/class/gpio/export");
    system("echo 147 > /sys/class/gpio/export");
    system("echo 144 > /sys/class/gpio/export");
    cout << "done: export" << endl;

    // Set as output pins
    system("echo out > /sys/class/gpio/gpio171/direction");
    system("echo out > /sys/class/gpio/gpio172/direction");
    system("echo out > /sys/class/gpio/gpio174/direction");
    system("echo out > /sys/class/gpio/gpio173/direction");
    system("echo out > /sys/class/gpio/gpio175/direction");
    system("echo out > /sys/class/gpio/gpio146/direction");
    system("echo out > /sys/class/gpio/gpio145/direction");
    system("echo out > /sys/class/gpio/gpio147/direction");
    system("echo out > /sys/class/gpio/gpio144/direction");
    cout << "done: set output" << endl;

    // Reset values to 0
    system("echo 0 > /sys/class/gpio/gpio171/value");
    system("echo 0 > /sys/class/gpio/gpio172/value");
    system("echo 0 > /sys/class/gpio/gpio174/value");
    system("echo 0 > /sys/class/gpio/gpio173/value");
    system("echo 0 > /sys/class/gpio/gpio175/value");
    system("echo 0 > /sys/class/gpio/gpio146/value");
    system("echo 0 > /sys/class/gpio/gpio145/value");
    system("echo 0 > /sys/class/gpio/gpio147/value");
    system("echo 0 > /sys/class/gpio/gpio144/value");
    cout << "done: reset values" << endl;
}

void setGpio(int pin){
    switch (pin)
    {
    case 1:
	    system("echo 1 > /sys/class/gpio/gpio145/value");
	    break;
	case 2:
	    system("echo 1 > /sys/class/gpio/gpio144/value");
	    break;
	case 3:
	    system("echo 1 > /sys/class/gpio/gpio146/value");
	    break;
	case 4:
	    system("echo 1 > /sys/class/gpio/gpio147/value");
	    break;
	case 5:
	    system("echo 1 > /sys/class/gpio/gpio173/value");
	    break;
	case 6:
	    system("echo 1 > /sys/class/gpio/gpio174/value");
	    break;
	case 7:
	    system("echo 1 > /sys/class/gpio/gpio172/value");
	    break;
	case 8:
	    system("echo 1 > /sys/class/gpio/gpio171/value");
	    break;
	case 9:
	    system("echo 1 > /sys/class/gpio/gpio175/value");
	    break;
    // Set all pins
    default:
        system("echo 1 > /sys/class/gpio/gpio171/value");
        system("echo 1 > /sys/class/gpio/gpio172/value");
        system("echo 1 > /sys/class/gpio/gpio174/value");
        system("echo 1 > /sys/class/gpio/gpio173/value");
        system("echo 1 > /sys/class/gpio/gpio175/value");
        system("echo 1 > /sys/class/gpio/gpio146/value");
        system("echo 1 > /sys/class/gpio/gpio145/value");
        system("echo 1 > /sys/class/gpio/gpio147/value");
        system("echo 1 > /sys/class/gpio/gpio144/value");
        break;
    }
}

void resetGpio(int pin){
    switch (pin)
    {
    case 1:
	    system("echo 0 > /sys/class/gpio/gpio145/value");
	    break;
	case 2:
	    system("echo 0 > /sys/class/gpio/gpio144/value");
	    break;
	case 3:
	    system("echo 0 > /sys/class/gpio/gpio146/value");
	    break;
	case 4:
	    system("echo 0 > /sys/class/gpio/gpio147/value");
	    break;
	case 5:
	    system("echo 0 > /sys/class/gpio/gpio173/value");
	    break;
	case 6:
	    system("echo 0 > /sys/class/gpio/gpio174/value");
	    break;
	case 7:
	    system("echo 0 > /sys/class/gpio/gpio172/value");
	    break;
	case 8:
	    system("echo 0 > /sys/class/gpio/gpio171/value");
	    break;
	case 9:
	    system("echo 0 > /sys/class/gpio/gpio175/value");
	    break;
    // Reset all pins
    default:
        system("echo 0 > /sys/class/gpio/gpio171/value");
        system("echo 0 > /sys/class/gpio/gpio172/value");
        system("echo 0 > /sys/class/gpio/gpio174/value");
        system("echo 0 > /sys/class/gpio/gpio173/value");
        system("echo 0 > /sys/class/gpio/gpio175/value");
        system("echo 0 > /sys/class/gpio/gpio146/value");
        system("echo 0 > /sys/class/gpio/gpio145/value");
        system("echo 0 > /sys/class/gpio/gpio147/value");
        system("echo 0 > /sys/class/gpio/gpio144/value");
        break;
    }
}
