#include "opencv2/opencv.hpp"
#include "dirent.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <sys/time.h>

#define SCALE_FACTOR    0.7
#define NUM_TEMPL       7
#define OPEN_DIR        true

using namespace cv;
using namespace std;

void getMatches(Mat result, Size s, vector<Rect> &matches, float thresh);
void scaleTempl(Mat templ, vector<Mat> &templates, vector<Size> &sizes);
void matchAllTemplates(Mat image, vector<Mat> templates, Point &largest_point, Size &largest_size, float threshold, int matchMethod);

int main(int argc, char** argv)
{
    Mat templ;
    Mat gray;

    int matchMethod = 5;
    float threshold = 0.5;

    cvtColor(imread("crop100.png", 1), templ, CV_RGB2GRAY);

    vector<Mat> templates;
    vector<Size> sizes;
    scaleTempl(templ, templates, sizes);

    if (OPEN_DIR)
    {
        int truePosCount = 0;
        int falsePosCount = 0;
        int trueNegCount = 0;
        int falseNegCount = 0;
        int totalCount = 0;
        
        Mat image;
        Point pt1;
        Point pt2;
        string fileName;
        size_t extentionPos;

        namedWindow("Image", 1);

        DIR *dir;
        struct dirent *ent;
        string path = "/Volumes/dtcristo/pp_resized/";
        dir = opendir(path.c_str());

        if (dir != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                stringstream ss;

                ss.str("");
                ss << path << ent->d_name;
                fileName = ss.str();

                extentionPos = fileName.find(".JPG");
                if (extentionPos != string::npos)
                {
                    image = imread(fileName);
                    if (!image.data) // Check for invalid input
                    {
                        cout << "Could not open or find the image" << endl;
                        return -1;
                    }

                    //------------ READ METADATA-------------
                    string txtFileName = fileName;
                    txtFileName.replace(extentionPos, 4, ".txt");
                    string word;
                    ifstream inFile(txtFileName.c_str());
                    if (inFile.is_open())
                    {
                        getline(inFile, word, ',');
                        pt1.x = atoi(word.c_str());
                        getline(inFile, word, ',');
                        pt1.y = atoi(word.c_str());
                        getline(inFile, word, ',');
                        pt2.x = atoi(word.c_str());
                        getline(inFile, word, ',');
                        pt2.y = atoi(word.c_str());
                        inFile.close();
                    }
                    else cout << "Unable to open .txt file" << endl;
                    
                    Rect trueArea(pt1, pt2);
                    //---------------------------------------------

                    cvtColor(image, gray, CV_RGB2GRAY);

                    Point matchTopLeft;
                    Size matchSize;
                    
                    matchAllTemplates(gray, templates, matchTopLeft, matchSize, threshold, matchMethod);
                    
                    Point matchCentroid;
                    matchCentroid.x = matchTopLeft.x + matchSize.width/2;
                    matchCentroid.y = matchTopLeft.y + matchSize.height/2;
                    
                    bool trueMatch = (matchCentroid.x >= pt1.x) && (matchCentroid.x <= pt2.x) && (matchCentroid.y >= pt1.y) && (matchCentroid.y <= pt1.y);

                    // Drawing functions
                    rectangle(image, trueArea, Scalar(255, 0, 0));
                    if (trueMatch)
                    {
                        rectangle(image, Rect(matchTopLeft, matchSize), Scalar(0, 255, 0));
                    }
                    else
                    {
                        rectangle(image, Rect(matchTopLeft, matchSize), Scalar(0, 0, 255));
                    }

                    imshow("Image", image);

                    for (;;)
                    {
                        int c = waitKey(1);
                        if ((char) c == 27 /*ESC key*/)
                        {
                            return 0;
                        }
                        else if ((char) c == ' ')
                        {
                            break;
                        }
                    }
                }
            }
            closedir(dir);
            cout << "Done!" << endl;
        }
        else
        {
            cout << "Could not open directory" << endl;
            return -1;
        }
    }
    else
    {
        Mat frame;
        
        VideoCapture capture(0);
        if (!capture.isOpened())
        {
            cout << "Could not open VideoCapture." << endl;
            return -1;
        }

        string frameWindow = "Webcam";
        string matchWindow = "Matches";
        namedWindow(frameWindow, CV_WINDOW_AUTOSIZE);
        namedWindow(matchWindow, CV_WINDOW_AUTOSIZE);

        int framecount = 0;
        timeval start;
        gettimeofday(&start, 0);
        for (;;)
        {
            capture >> frame;
            imshow(frameWindow, frame);
            cvtColor(frame, gray, CV_RGB2GRAY);

            Point largest_point;
            Size largest_size;
            matchAllTemplates(gray, templates, largest_point, largest_size, threshold, matchMethod);

            cvtColor(gray, gray, CV_GRAY2RGB);

            rectangle(gray, Rect(largest_point, largest_size), Scalar(255, 0, 0));

            //        for (vector<Rect>::iterator it = matches.begin(); it != matches.end(); ++it)
            //            rectangle(gray, *it, Scalar(255, 0, 0));

            imshow(matchWindow, gray);

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
            else if ((char) c == 'x')
            {
                threshold += 0.01;
                cout << "threshold +0.01 = " << threshold << endl;
            }
            else if ((char) c == 'z')
            {
                threshold -= 0.01;
                cout << "threshold -0.01 = " << threshold << endl;
            }
            else if ((char) c == '1')
            {
                matchMethod = 1;
                cout << "matchMethod = SQDIFF (1)" << endl;
            }
            else if ((char) c == '2')
            {
                matchMethod = 2;
                cout << "matchMethod = SQDIFF NORMED (2)" << endl;
            }
            else if ((char) c == '3')
            {
                matchMethod = 3;
                cout << "matchMethod = TM CCORR (3)" << endl;
            }
            else if ((char) c == '4')
            {
                matchMethod = 4;
                cout << "matchMethod = TM COEFF (4)" << endl;
            }
            else if ((char) c == '5')
            {
                matchMethod = 5;
                cout << "matchMethod = TM COEFF NORMED (5)" << endl;
            }
            framecount++;
        }
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

void matchAllTemplates(Mat image, vector<Mat> templates, Point &largest_point, Size &largest_size, float threshold, int matchMethod)
{
    //vector<Rect> matches;
    float largest_val = 0;

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
        }

        //if (matches.size() < 5)
        //    getMatches(result, s, matches, threshold);
    }
    //gettimeofday(&b, 0);
    //cout << "Time in msec = " << (b.tv_usec - a.tv_usec)/1000 << endl;
}