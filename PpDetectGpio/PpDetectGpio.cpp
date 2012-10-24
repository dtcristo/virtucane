#include "opencv2/opencv.hpp"
#include "dirent.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <sys/time.h>

#define SCALE_FACTOR    0.7
#define NUM_TEMPL       3

using namespace cv;
using namespace std;

void getMatches(Mat result, Size s, vector<Rect> &matches, float thresh);
void scaleTempl(Mat templ, vector<Mat> &templates, vector<Size> &sizes);
bool matchAllTemplates(Mat image, vector<Mat> templates, Point &largest_point, Size &largest_size, float threshold, int matchMethod);
void setGpio(int pin);
void resetGpio(int pin);

ofstream resultsFile;

int main(int argc, char** argv)
{
    namedWindow("Image", 1);
    
    Mat templ;
    Mat gray;

    int matchMethod = 5;
    float threshold = 0.53;
    
    int old = 0;
    int current = 0;

    cvtColor(imread("crop25.png", 1), templ, CV_RGB2GRAY);

    vector<Mat> templates;
    vector<Size> sizes;
    scaleTempl(templ, templates, sizes);

    Mat frame;
    
    VideoCapture capture(0);
    if (!capture.isOpened())
    {
        cout << "Could not open VideoCapture." << endl;
        return -1;
    }

    int framecount = 0;
    timeval start;
    gettimeofday(&start, 0);
    for (;;)
    {
        capture >> frame;
        cvtColor(frame, gray, CV_RGB2GRAY);

        Point largest_point;
        Size largest_size;
        if (matchAllTemplates(gray, templates, largest_point, largest_size, threshold, matchMethod))
        {
            rectangle(frame, Rect(largest_point, largest_size), Scalar(255, 0, 0));
            
            int x = largest_point.x;
            int y = largest_point.y;
            
            //LEFT
            if (x<213)
            {
                //TOP
                if (y<160)
                {
                    rectangle(frame, Point(0,0), Point(213,160), Scalar(0, 0, 255), 3);
                    //setGpio(1);
                    //system("osascript 1on.scpt");
                    current = 1;
                }
                //BOTTOM
                else if (y>320)
                {
                    rectangle(frame, Point(0,320), Point(213,480), Scalar(0, 0, 255), 3);
                    //setGpio(3);
                    //system("osascript 3on.scpt");
                    current = 3;
                }
                //MID
                else
                {
                    rectangle(frame, Point(0,160), Point(213,320), Scalar(0, 0, 255), 3);
                    //setGpio(2);
                    //system("osascript 2on.scpt");
                    current = 2;
                }
            }
            //RIGHT
            else if (x>427)
            {
                //TOP
                if (y<160)
                {
                    rectangle(frame, Point(427,0), Point(640,160), Scalar(0, 0, 255), 3);
                    //setGpio(7);
                    //system("osascript 7on.scpt");
                    current = 7;
                }
                //BOTTOM
                else if (y>320)
                {
                    rectangle(frame, Point(427,320), Point(640,480), Scalar(0, 0, 255), 3);
                    //setGpio(9);
                    //system("osascript 9on.scpt");
                    current = 9;
                }
                //MID
                else
                {
                    rectangle(frame, Point(427,160), Point(640,320), Scalar(0, 0, 255), 3);
                    //setGpio(8);
                    //system("osascript 8on.scpt");
                    current = 8;
                }
            }
            //CENTRE
            else
            {
                //TOP
                if (y<160)
                {
                    rectangle(frame, Point(213,0), Point(427,160), Scalar(0, 0, 255), 3);
                    //setGpio(4);
                    //system("osascript 4on.scpt");
                    current = 4;
                }
                //BOTTOM
                else if (y>320)
                {
                    rectangle(frame, Point(213,320), Point(427,480), Scalar(0, 0, 255), 3);
                    //setGpio(6);
                    //system("osascript 6on.scpt");
                    current = 6;
                }
                //MID
                else
                {
                    rectangle(frame, Point(213,160), Point(427,320), Scalar(0, 0, 255), 3);
                    //setGpio(5);
                    //system("osascript 5on.scpt");
                    current = 5;
                }
            }
        }
        else
        {
            //system("osascript alloff.scpt");
            current = 0;
        }

        
        if (old != current)
        {
            setGpio(current);
            resetGpio(old);
        }
        
        old = current;

        //        for (vector<Rect>::iterator it = matches.begin(); it != matches.end(); ++it)
        //            rectangle(gray, *it, Scalar(255, 0, 0));

        imshow("Image", frame);

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
        framecount++;
    }
    return 0;
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

void setGpio(int pin)
{
    switch (pin)
        {
        case 1:
            system("osascript 1on.scpt");
            break;
        case 2:
            system("osascript 2on.scpt");
            break;
        case 3:
            system("osascript 3on.scpt");
            break;
        case 4:
            system("osascript 4on.scpt");
            break;
        case 5:
            system("osascript 5on.scpt");
            break;
        case 6:
            system("osascript 6on.scpt");
            break;
        case 7:
            system("osascript 7on.scpt");
            break;
        case 8:
            system("osascript 8on.scpt");
            break;
        case 9:
            system("osascript 9on.scpt");
            break;
        default:
            break;
        }
}

void resetGpio(int pin)
{
    switch (pin)
        {
        case 1:
            system("osascript 1off.scpt");
            break;
        case 2:
            system("osascript 2off.scpt");
            break;
        case 3:
            system("osascript 3off.scpt");
            break;
        case 4:
            system("osascript 4off.scpt");
            break;
        case 5:
            system("osascript 5off.scpt");
            break;
        case 6:
            system("osascript 6off.scpt");
            break;
        case 7:
            system("osascript 7off.scpt");
            break;
        case 8:
            system("osascript 8off.scpt");
            break;
        case 9:
            system("osascript 9off.scpt");
            break;
        default:
            break;
        }
}
