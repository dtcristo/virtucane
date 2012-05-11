#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat lolcat;
bool first;
Point pt1;
Point pt2;

void
onMouse(int event, int x, int y, int, void*)
{
    if (event != CV_EVENT_LBUTTONDOWN)
        return;

    if (first)
    {
        first = false;
        pt1 = Point(x, y);
    }
    else
    {
        first = true;
        pt2 = Point(x, y);
        rectangle(lolcat, pt1, pt2, Scalar(255, 0, 0), 3);
        imshow("Image", lolcat);
    }
}

int
main(int, char**)
{
    namedWindow("Image", 1);
    lolcat = imread("lol.jpg");
    imshow("Image", lolcat);
    setMouseCallback("Image", onMouse, 0);

    for (;;)
    {
        int c = waitKey(1);
        if ((char) c == 27 /*ESC key*/)
        {
            break;
        }
        else if ((char) c == ' ')
        {
            first = true;
            lolcat = imread("lol.jpg");
            imshow("Image", lolcat);
        }
    }

    return 0;
}
