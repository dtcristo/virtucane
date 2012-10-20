#include "video.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <ctime>
#include <sys/time.h>

#include <iostream>

#define SCALE_FACTOR    0.7
#define NUM_TEMPL       3

using namespace std;
using namespace cv;

int main();
void initGpio();
void setGpio(int pin);
void resetGpio(int pin);
void actuateMotors(int x, int y);

void getMatches(Mat result, Size s, vector<Rect> &matches, float thresh);
void scaleTempl(Mat templ, vector<Mat> &templates, vector<Size> &sizes);
bool matchAllTemplates(Mat image, vector<Mat> templates, Point &largest_point, Size &largest_size, float threshold, int matchMethod);

int main()
{
    initGpio();
    
    Mat templ;
    Mat gray;
    
    int matchMethod = 5;
    float threshold = 0.53;
    
    cvtColor(imread("crop25.png", 1), templ, CV_RGB2GRAY);
    
    vector<Mat> templates;
    vector<Size> sizes;
    scaleTempl(templ, templates, sizes);

    while(video_init((char*) "/dev/video0", 640, 480, 30))
    {
        printf("Camera initialisation failed. Retry in 2 seconds...\n");
        sleep(2);
    }
    
    unsigned char* frame = new unsigned char[640*480*2];
    
    sleep(5);
    
    int framecount = 0;
    timeval start;
    gettimeofday(&start, 0);
    
    while(framecount < 100)
    {
        get_frame_grey(frame);
        Mat cvFrame = Mat(480, 640, CV_8UC1, (void*) frame);
        
        Point largest_point;
        Size largest_size;
        matchAllTemplates(cvFrame, templates, largest_point, largest_size, threshold, matchMethod);
        
        actuateMotors(largest_point.x, largest_point.y);
        
        framecount++;
        cout << "Frame: " << framecount << endl;
        
        while(!video_frame_ready())
            usleep(50);
    }
    
    timeval end;
    gettimeofday(&end, 0);
    int seconds = (end.tv_sec - start.tv_sec);
    int fps = framecount / seconds + 0.5;
    cout << "fps = " << fps << endl;
    
    //FILE *file_fd;
    //file_fd = fopen("video_output.yuv", "wb");
    //get_frame_grey(frame);
    //fwrite((const void *) frame, 640*480, 1, file_fd);
    //fclose(file_fd);
    
    video_close();
    
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
        system("echo 1 > /sys/class/gpio/gpio171/value");
        break;
    case 2:
        system("echo 1 > /sys/class/gpio/gpio172/value");
        break;
    case 3:
        system("echo 1 > /sys/class/gpio/gpio174/value");
        break;
    case 4:
        system("echo 1 > /sys/class/gpio/gpio173/value");
        break;
    case 5:
        system("echo 1 > /sys/class/gpio/gpio175/value");
        break;
    case 6:
        system("echo 1 > /sys/class/gpio/gpio146/value");
        break;
    case 7:
        system("echo 1 > /sys/class/gpio/gpio145/value");
        break;
    case 8:
        system("echo 1 > /sys/class/gpio/gpio147/value");
        break;
    case 9:
        system("echo 1 > /sys/class/gpio/gpio144/value");
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
        system("echo 0 > /sys/class/gpio/gpio171/value");
        break;
    case 2:
        system("echo 0 > /sys/class/gpio/gpio114/value");
        break;
    case 3:
        system("echo 0 > /sys/class/gpio/gpio172/value");
        break;
    case 4:
        system("echo 0 > /sys/class/gpio/gpio174/value");
        break;
    case 5:
        system("echo 0 > /sys/class/gpio/gpio173/value");
        break;
    case 6:
        system("echo 0 > /sys/class/gpio/gpio175/value");
        break;
    case 7:
        system("echo 0 > /sys/class/gpio/gpio151/value");
        break;
    case 8:
        system("echo 0 > /sys/class/gpio/gpio148/value");
        break;
    case 9:
        system("echo 0 > /sys/class/gpio/gpio186/value");
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

void actuateMotors(int x, int y)
{
    //LEFT
    if (x<213)
    {
        //TOP
        if (y<160)
        {
            //7
            cout << "TOP LEFT" << endl;
            setGpio(1);
        }
        //BOTTOM
        else if (y>320)
        {
            //1
            cout << "BOTTOM LEFT" << endl;
            setGpio(3);
        }
        //MID
        else
        {
            //4
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
            //9
            cout << "TOP RIGHT" << endl;
            setGpio(7);
        }
        //BOTTOM
        else if (y>320)
        {
            //3
            cout << "BOTTOM RIGHT" << endl;
            setGpio(9);
        }
        //MID
        else
        {
            //6
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
            //8
            cout << "TOP CENTRE" << endl;
            setGpio(4);
        }
        //BOTTOM
        else if (y>320)
        {
            //2
            cout << "BOTTOM CENTRE" << endl;
            setGpio(6);
        }
        //MID
        else
        {
            //5
            cout << "MID CENTRE" << endl;
            setGpio(5);
        }
    }
}

void getMatches(Mat result, Size s, vector<Rect> &matches, float thresh)
{
    assert(result.channels() == 1);

    for (int y = 0; y < result.rows; y++)
    {
        for (int x = 0; x < result.cols; x++)
        {
            if (result.at<float>(Point(x, y)) >= thresh)
                matches.push_back(Rect(Point(x, y), s));
        }
    }
}

void scaleTempl(Mat templ, vector<Mat> &templates, vector<Size> &sizes)
{
    Size s(templ.cols, templ.rows);

    templates.push_back(templ);
    sizes.push_back(s);

    for (int i = 1; i < NUM_TEMPL; i++)
    {
        sizes.push_back(Size(SCALE_FACTOR * sizes[i - 1].width, SCALE_FACTOR * sizes[i - 1].height));
    }

    for (vector<Size>::iterator it = sizes.begin() + 1; it != sizes.end(); ++it)
    {
        Mat shrunk;
        resize(templ, shrunk, *it);
        templates.push_back(shrunk);
    }
}

bool matchAllTemplates(Mat image, vector<Mat> templates, Point &largest_point, Size &largest_size, float threshold, int matchMethod)
{
    //vector<Rect> matches;
    float largest_val = 0;
    bool retval = false;

    //timeval a;
    //timeval b;
    //gettimeofday(&a, 0);
    for (vector<Mat>::iterator it = templates.begin(); it != templates.end(); ++it)
    {
        Mat result;
        Mat resultNorm;
        Size s(it->cols, it->rows);

        int resultCols = image.cols - it->cols + 1;
        int resultRows = image.rows - it->rows + 1;
        result.create(resultCols, resultRows, CV_32FC1);

        matchTemplate(image, *it, result, matchMethod);

        //normalize(result, resultNorm, 0, 1, NORM_MINMAX, -1, Mat());
        //cvtColor(resultNorm, resultNorm, CV_GRAY2RGB);

        double minVal;
        double maxVal;
        Point minLoc;
        Point maxLoc;

        minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

        float current = result.at<float>(maxLoc);
        if ((current >= threshold) && (current > largest_val))
        {
            largest_val = current;
            largest_point = maxLoc;
            largest_size = s;
            retval = true;
        }

        //if (matches.size() < 5)
        //    getMatches(result, s, matches, threshold);
    }
    return retval;
    //gettimeofday(&b, 0);
    //cout << "Time in msec = " << (b.tv_usec - a.tv_usec)/1000 << endl;
}
