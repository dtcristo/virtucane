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
    int match_method = 5;
    float threshold = 0.4;
    int numTempl = 4;
    float scaleFactor = 0.6;

    Mat templ;
    Mat frame;
    Mat gray;
    Mat binary;
    Mat scene;
    //Mat result;
    //Mat result_norm;
    Mat img_display;

    VideoCapture capture(0);
    if (!capture.isOpened())
        return -1;

    string frame_window = "Webcam";
    string image_window = "Detected";
    //string result_window = "Result";
    namedWindow(frame_window, CV_WINDOW_AUTOSIZE);
    namedWindow(image_window, CV_WINDOW_AUTOSIZE);
    //namedWindow(result_window, CV_WINDOW_AUTOSIZE);

    cvtColor(imread("templ.png", 1), templ, CV_RGB2GRAY);
    //Size s(templ.cols, templ.rows);

    vector<Mat> templates;
    vector<Size> sizes;
    scaleTempl(templ, templates, sizes, scaleFactor, numTempl);

    for (;;)
    {
        capture.grab();
        capture.retrieve(frame);
        imshow(frame_window, frame);

        cvtColor(frame, gray, CV_RGB2GRAY);
        //threshold(gray, binary, 10, 255, THRESH_OTSU);
        //cvtColor(binary, binary, CV_GRAY2RGB);

        scene = gray;

        int c = waitKey(30);
        if ((char) c == 27 /*esc key*/)
        {
            break;
        }
        else if ((char) c == 'x')
        {
            threshold += 0.01;
            cout << "Threshold +0.01 = " << threshold << endl;
        }
        else if ((char) c == 'z')
        {
            threshold -= 0.01;
            cout << "Threshold -0.01 = " << threshold << endl;
        }
        else if ((char) c == '1')
        {
            match_method = 1;
            cout << "match_method = SQDIFF (" << match_method << ")" << endl;
        }
        else if ((char) c == '2')
        {
            match_method = 2;
            cout << "match_method = SQDIFF NORMED (" << match_method << ")" << endl;
        }
        else if ((char) c == '3')
        {
            match_method = 3;
            cout << "match_method = TM CCORR (" << match_method << ")" << endl;
        }
        else if ((char) c == '4')
        {
            match_method = 4;
            cout << "match_method = TM COEFF (" << match_method << ")" << endl;
        }
        else if ((char) c == '5')
        {
            match_method = 5;
            cout << "match_method = TM COEFF NORMED (" << match_method << ")" << endl;
        }
        else if ((char) c == 'r')
        {
            scene.copyTo(img_display);
            cvtColor(img_display, img_display, CV_GRAY2RGB);

            vector<Rect> matches;

            cout << "Processing current frame" << endl;
            int x = 0;
            for (vector<Mat>::iterator it = templates.begin(); it != templates.end(); ++it)
            {
                Mat result;
                Mat result_norm;
                Size s(it->cols, it->rows);

                int result_cols = scene.cols - it->cols + 1;
                int result_rows = scene.rows - it->rows + 1;
                result.create(result_cols, result_rows, CV_32FC1);

                matchTemplate(scene, *it, result, match_method);

                normalize(result, result_norm, 0, 1, NORM_MINMAX, -1, Mat());
                cvtColor(result_norm, result_norm, CV_GRAY2RGB);

                double minVal;
                double maxVal;
                Point minLoc;
                Point maxLoc;
                Point matchLoc;

                minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
                //matchedPoints.push_back(maxLoc);
                cout << "maxVal (scale " << x << ") = " << result.at<float>(maxLoc) << endl;

                getMatches(result, s, matches, threshold);

//                if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
//                    getMatches(result, s, matches, threshold, false);
//                else
//                    getMatches(result, s, matches, threshold, true);

                cout << "matches.size() (scale " << x << ") = " << matches.size() << endl;

                x++;
            }
            cout << endl;

            for (vector<Rect>::iterator it = matches.begin(); it != matches.end(); ++it)
            {
                rectangle(img_display, *it, Scalar(0, 0, 255));
                //circle(result_norm, it->tl(), 1, Scalar(0, 0, 255));
            }

            imshow(image_window, img_display);
            //imshow(result_window, result_norm);
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
