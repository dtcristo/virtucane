/**
 * @file MatchTemplate_Demo.cpp
 * @brief Sample code to use the function MatchTemplate
 * @author OpenCV team
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat binary;
Mat templ;
Mat result;
string image_window = "Source Image";
string result_window = "Result window";

int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod(int, void*);

/**
 * @function main
 */
int main(int argc, char** argv) {

	Mat frame, gray;

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

		binary = gray;
		//threshold(gray, binary, 10, 255, THRESH_OTSU);

		//imshow(window_name, binary);

//		if (!frame.empty()) {
//			detectAndDisplay(frame);
//		} else {
//			//printf("Error: No captured frame -- Break!\n");
//			//break;
//		}

		MatchingMethod(0, 0);

		if (waitKey(30) >= 0)
			break;
	}

	return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod(int, void*) {
	/// Source image to display
	Mat img_display;
	binary.copyTo(img_display);

	/// Create the result matrix
	int result_cols = binary.cols - templ.cols + 1;
	int result_rows = binary.rows - templ.rows + 1;

	result.create(result_cols, result_rows, CV_32FC1);

	/// Do the Matching and Normalize
	matchTemplate(binary, templ, result, match_method);
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
