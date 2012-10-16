#include "opencv2/opencv.hpp"
#include "dirent.h"
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

Mat image;
bool first;
bool done;
Point pt1;
Point pt2;
string fileName;
size_t extentionPos;

void
onMouse(int event, int x, int y, int, void*)
{
    if (event != CV_EVENT_LBUTTONDOWN)
        return;

    if (first)
    {
        first = false;
        done = false;
        pt1 = Point(x, y);
    }
    else
    {
        first = true;
        if ((x - pt1.x) >= (y - pt1.y))
        {
            y = pt1.y + (x - pt1.x);
        }
        else
        {
            x = pt1.x + (y - pt1.y);
        }
        pt2 = Point(x, y);
        rectangle(image, pt1, pt2, Scalar(255, 0, 0));
        imshow("Image", image);
        done = true;
    }
}

int
main(int, char**)
{
    namedWindow("Image", 1);
    cvSetMouseCallback("Image", onMouse);

    DIR *dir;
    struct dirent *ent;
    string path = "/Volumes/dtcristo/pp_resized/";
    dir = opendir(path.c_str());
    
    string outFileName = "info.dat";
    ofstream outFile;
    outFile.open(outFileName.c_str(), ios::app);
    
    string bgFileName = "bg.txt";
    ofstream bgFile;
    bgFile.open(bgFileName.c_str(), ios::app);

    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            stringstream ss;

            ss.str("");
            ss << path << ent->d_name;
            fileName = ss.str();
            cout << fileName << endl;
            extentionPos = fileName.find(".JPG");
            if (extentionPos != string::npos)
            {
                done = false;
                image = imread(fileName);
                if (!image.data) // Check for invalid input
                {
                    cout << "Could not open or find the image" << endl;
                    return -1;
                }
                imshow("Image", image);

                for (;;)
                {
                    int c = waitKey(1);
                    if ((char) c == 27 /*ESC key*/)
                    {
                        outFile.close();
                        bgFile.close();
                        return 0;
                    }
                    else if ((char) c == 'c')
                    {
                        first = true;
                        done = false;
                        cout << "Data cleared" << endl;
                        image = imread(fileName);
                        imshow("Image", image);
                    }
                    else if ((char) c == 's')
                        break;
                    else if ((char) c == 'b')
                    {
                        bgFile << fileName << endl;
                        break;
                    }
                    else if ((char) c == ' ')
                    {
                        if (done)
                        {
                            int width = pt2.x - pt1.x;
                            outFile << fileName << "  1  " << pt1.x << " " << pt1.y << " " << width << " " << width << endl;
                            cout    << fileName << "  1  " << pt1.x << " " << pt1.y << " " << width << " " << width << endl;
                            break;
                        }
                    }
                }
            }
        }
        closedir(dir);
        outFile.close();
        bgFile.close();
        cout << "Done!" << endl;
        return 0;
    }
    else
    {
        cout << "Could not open directory" << endl;
        return -1;
    }

    return 0;
}
