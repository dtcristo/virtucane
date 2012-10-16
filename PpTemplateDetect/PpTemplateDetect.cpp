#include "opencv2/opencv.hpp"
#include "dirent.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <sys/time.h>

#define SCALE_FACTOR    0.7
#define NUM_TEMPL       3
#define OPEN_DIR        false

using namespace cv;
using namespace std;

void getMatches(Mat result, Size s, vector<Rect> &matches, float thresh);
void scaleTempl(Mat templ, vector<Mat> &templates, vector<Size> &sizes);
bool matchAllTemplates(Mat image, vector<Mat> templates, Point &largest_point, Size &largest_size, float threshold, int matchMethod);
void printResults(float threshold, int TN, int TP, int FN, int FP, int TG, int TF);
void saveResults(float threshold, int TN, int TP, int FN, int FP, int TG, int TF);

ofstream resultsFile;

int main(int argc, char** argv)
{
    namedWindow("Image", 1);
    
    Mat templ;
    Mat gray;

    int matchMethod = 5;
    float threshold = 0.53;

    cvtColor(imread("crop25.png", 1), templ, CV_RGB2GRAY);

    vector<Mat> templates;
    vector<Size> sizes;
    scaleTempl(templ, templates, sizes);

    if (OPEN_DIR)
    {
        //string resultsFileName = "results.csv";
        //resultsFile.open(resultsFileName.c_str(), ios::app);
        //resultsFile << "Threshold," << "Tracker Detection Rate," << "False Alarm Rate," << "Detection Rate," << "Specificity," << "Accuracy," << "Positive Prediction," << "Negative Prediction," << "False Negative Rate," << "False Positive Rate" << endl;
        
        
        //for (threshold=0.4; threshold<.7; threshold+=0.025)
        {
            int TN = 0;
            int TP = 0;
            int FN = 0;
            int FP = 0;
            int TG = 0;
            int TF = 0;

            bool isPP;

            Mat image;
            Point pt1;
            Point pt2;
            string fileName;
            size_t extentionPos;

            DIR *dir;
            struct dirent *ent;
            string path = "/Volumes/dtcristo/pp_resized/";
            dir = opendir(path.c_str());

            if (dir != NULL)
            {
                int framecount = 0;
                timeval start;
                gettimeofday(&start, 0);

                while ((ent = readdir(dir)) != NULL)
                {
                    stringstream ss;

                    ss.str("");
                    ss << path << ent->d_name;
                    fileName = ss.str();

                    extentionPos = fileName.find(".JPG");
                    if (extentionPos != string::npos)
                    {
                        TF++;

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

                            isPP = true;
                            TG++;
                        }
                        else
                        {
                            isPP = false;
                        }

                        Rect trueArea(pt1, pt2);

                        //---------------------------------------------

                        cvtColor(image, gray, CV_RGB2GRAY);

                        Point matchTopLeft;
                        Size matchSize;
                        bool foundMatch = matchAllTemplates(gray, templates, matchTopLeft, matchSize, threshold, matchMethod);

                        if (isPP) rectangle(image, trueArea, Scalar(255, 0, 0));

                        if (!foundMatch)
                        {
                            if (isPP)
                                FN++;
                            else
                                TN++;
                        }
                        else
                        {
                            Point matchCentroid;
                            matchCentroid.x = matchTopLeft.x + matchSize.width/2;
                            matchCentroid.y = matchTopLeft.y + matchSize.height/2;

                            if (matchCentroid.inside(trueArea))
                            {
                                TP++;
                                rectangle(image, Rect(matchTopLeft, matchSize), Scalar(0, 255, 0));
                            }
                            else
                            {
                                FP++;
                                rectangle(image, Rect(matchTopLeft, matchSize), Scalar(0, 0, 255));
                            }
                        }

//                        imshow("Image", image);
//
//                        for (;;)
//                        {
//                            int c = waitKey(1);
//                            if ((char) c == 27 /*ESC key*/)
//                            {
//                                return 0;
//                            }
//                            else if ((char) c == ' ')
//                            {
//                                break;
//                            }
//                        }
                    }
                    framecount++;
                }
                timeval end;
                gettimeofday(&end, 0);
                int seconds = (end.tv_sec - start.tv_sec);
                int fps = framecount / seconds + 0.5;
                cout << "fps = " << fps << endl;

                closedir(dir);
                printResults(threshold, TN, TP, FN, FP, TG, TF);
                //saveResults(threshold, TN, TP, FN, FP, TG, TF);
            }
            else
            {
                cout << "Could not open directory" << endl;
                return -1;
            }
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

        int framecount = 0;
        timeval start;
        gettimeofday(&start, 0);
        for (;;)
        {
            capture >> frame;
            cvtColor(frame, gray, CV_RGB2GRAY);

            Point largest_point;
            Size largest_size;
            matchAllTemplates(gray, templates, largest_point, largest_size, threshold, matchMethod);

            rectangle(frame, Rect(largest_point, largest_size), Scalar(255, 0, 0));

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

void printResults(float threshold, int TN, int TP, int FN, int FP, int TG, int TF)
{
    cout << "----------------" << endl;
    cout << "PRINTING RESULTS" << endl;
    cout << "----------------" << endl;
    cout << endl;
    cout << "Threshold              = " << threshold << endl;
    cout << "Tracker Detection Rate = " << (float)TP/TG << endl;
    cout << "False Alarm Rate =       " << (float)FP/(TP+FP) << endl;
    cout << "Detection Rate =         " << (float)TP/(TP+FN) << endl;
    cout << "Specificity =            " << (float)TN/(FP+TN) << endl;
    cout << "Accuracy =               " << (float)(TP+TN)/TF << endl;
    cout << "Positive Prediction =    " << (float)TP/(TP+FP) << endl;
    cout << "Negative Prediction =    " << (float)TN/(FN+TN) << endl;
    cout << "False Negative Rate =    " << (float)FN/(FN+TP) << endl;
    cout << "False Positive Rate =    " << (float)FP/(FP+TN) << endl;
}

void saveResults(float threshold, int TN, int TP, int FN, int FP, int TG, int TF)
{
    resultsFile << threshold << "," << (float)TP/TG << "," << (float)FP/(TP+FP) << "," << (float)TP/(TP+FN) << "," << (float)TN/(FP+TN) << "," << (float)(TP+TN)/TF << "," << (float)TP/(TP+FP) << "," << (float)TN/(FN+TN) << "," << (float)FN/(FN+TP) << "," << (float)FP/(FP+TN) << endl;
}