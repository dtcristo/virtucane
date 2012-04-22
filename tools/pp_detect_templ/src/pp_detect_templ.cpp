/**
 * @file MatchTemplate_Demo.cpp
 * @brief Sample code to use the function MatchTemplate
 * @author OpenCV team
 */

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/// Global Variables
Mat scene;
Mat templ;
Mat result;
string image_window = "Source Image";
string result_window = "Result window";

int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod();
vector<Point> getMatches(Mat I, int thresh, bool max);

/**
 * @function main
 */
int main(int argc, char** argv) {

	int threshold = 100;
	Mat frame, gray, binary;

	VideoCapture capture(0);
	if (!capture.isOpened())
		return -1;

	templ = imread("templ.jpg", 1);

	namedWindow(image_window, CV_WINDOW_AUTOSIZE);
	namedWindow(result_window, CV_WINDOW_AUTOSIZE);

	/// Create Trackbar
	string trackbar_label =
			"Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
	createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar,
			NULL);

	for (;;) {

		capture.grab();
		capture.retrieve(frame);

		cvtColor(frame, gray, CV_RGB2GRAY);
		//threshold(gray, binary, 10, 255, THRESH_OTSU);
		//cvtColor(binary, binary, CV_GRAY2RGB);

		scene = frame;

		//imshow(image_window, gray);

		//MatchingMethod();

		int c = waitKey(30);
		if ((char) c == 'q') {
			break;
		} else if ((char) c == 'x') {
			threshold += 10;
			cout << "Threshold +10 = " << threshold << endl;
		} else if ((char) c == 'z') {
			threshold -= 10;
			cout << "Threshold -10 = " << threshold << endl;
		} else if ((char) c == 'p') {

			cout << "\nPrinting points" << endl;
			vector<Point> matchedPoints = getMatches(gray, threshold, false);

			for (int i = 0; i < matchedPoints.size(); i++) {
				cout << "x = " << matchedPoints[i].x << "\ty = "
						<< matchedPoints[i].y << endl;
				circle(frame, matchedPoints[i], 5, Scalar(0, 0, 255));

			}
			imshow(image_window, frame);
		}
	}

	return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod() {
	/// Source image to display
	Mat img_display;
	scene.copyTo(img_display);

	/// Create the result matrix
	int result_cols = scene.cols - templ.cols + 1;
	int result_rows = scene.rows - templ.rows + 1;

	result.create(result_cols, result_rows, CV_32FC1);

	/// Do the Matching and Normalize
	matchTemplate(scene, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal;
	double maxVal;
	Point minLoc;
	Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
		matchLoc = minLoc;
	} else {
		matchLoc = maxLoc;
	}

	/// Show me what you got
	rectangle(img_display, matchLoc,
			Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
			Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc,
			Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
			Scalar::all(0), 2, 8, 0);

	imshow(image_window, img_display);
	imshow(result_window, result);

	return;
}

vector<Point> getMatches(Mat I, int thresh, bool max = true) {

	// accept only char type matrices
	CV_Assert(I.depth() != sizeof(uchar));

	assert(I.channels() == 1);

	vector<Point> matchPoints;

	int nRows = I.rows;
	int nCols = I.cols;

	int i, j;
	uchar* p;
	for (i = 0; i < nRows; ++i) {
		p = I.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j) {
			if (max) {
				if (p[j] >= thresh) {
					matchPoints.push_back(Point(j, i));
				}
			} else {
				if (p[j] <= thresh) {
					matchPoints.push_back(Point(j, i));
				}
			}
		}
	}

	return matchPoints;
}
