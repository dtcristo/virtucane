#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void
getMatches(Mat result, Size s, vector<Rect> &matches, float thresh);
void
scaleTempl(Mat templ, vector<Mat> &templates, vector<Size> &sizes, float factor, int num);

int
main(int argc, char** argv)
{
    int matchMethod = 5;
    float threshold = 0.4;
    int numTempl = 6;
    float scaleFactor = 0.7;

    Mat templ;
    Mat frame;
    Mat gray;

    VideoCapture capture(0);
    if (!capture.isOpened())
        return -1;

    string frameWindow = "Webcam";
    string matchWindow = "Matches";
    namedWindow(frameWindow, CV_WINDOW_AUTOSIZE);
    namedWindow(matchWindow, CV_WINDOW_AUTOSIZE);

    cvtColor(imread("templ.png", 1), templ, CV_RGB2GRAY);

    vector<Mat> templates;
    vector<Size> sizes;
    scaleTempl(templ, templates, sizes, scaleFactor, numTempl);

    for (;;)
    {
        capture >> frame;
        imshow(frameWindow, frame);
        cvtColor(frame, gray, CV_RGB2GRAY);

        vector<Rect> matches;

        for (vector<Mat>::iterator it = templates.begin(); it != templates.end(); ++it)
        {
            Mat result;
            Mat resultNorm;
            Size s(it->cols, it->rows);

            int resultCols = gray.cols - it->cols + 1;
            int resultRows = gray.rows - it->rows + 1;
            result.create(resultCols, resultRows, CV_32FC1);

            matchTemplate(gray, *it, result, matchMethod);

//            normalize(result, resultNorm, 0, 1, NORM_MINMAX, -1, Mat());
//            cvtColor(resultNorm, resultNorm, CV_GRAY2RGB);

//            double minVal;
//            double maxVal;
//            Point minLoc;
//            Point maxLoc;

//            minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
//            matchedPoints.push_back(maxLoc);

            getMatches(result, s, matches, threshold);
        }

        cvtColor(gray, gray, CV_GRAY2RGB);

        for (vector<Rect>::iterator it = matches.begin(); it != matches.end(); ++it)
            rectangle(gray, *it, Scalar(0, 0, 255));

        imshow(matchWindow, gray);

        // Wait 30ms and poll keystrokes.
        int c = waitKey(30);
        if ((char) c == 27 /*ESC key*/)
        {
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
    }

    return 0;
}

void
getMatches(Mat result, Size s, vector<Rect> &matches, float thresh)
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

void
scaleTempl(Mat templ, vector<Mat> &templates, vector<Size> &sizes, float factor, int num)
{
    Size s(templ.cols, templ.rows);

    templates.push_back(templ);
    sizes.push_back(s);

    for (int i = 1; i < num; i++)
    {
        sizes.push_back(Size(factor * sizes[i - 1].width, factor * sizes[i - 1].height));
    }

    for (vector<Size>::iterator it = sizes.begin() + 1; it != sizes.end(); ++it)
    {
        Mat shrunk;
        resize(templ, shrunk, *it);
        templates.push_back(shrunk);
    }
}
