#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

string frame_window = "Webcam";
string image_window = "Detected";
string result_window = "Result";

vector<Point> getMatches(Mat I, float thresh, bool max);

int main(int argc, char** argv) {

	int match_method = 5;
	float threshold = 0.5;

	Mat templ;
	Mat frame;
	Mat gray;
	Mat binary;
	Mat scene;
	Mat result;
	Mat result_norm;
	Mat img_display;

	VideoCapture capture(0);
	if (!capture.isOpened())
		return -1;

	templ = imread("templ.jpg", 1);

	namedWindow(frame_window, CV_WINDOW_AUTOSIZE);
	namedWindow(image_window, CV_WINDOW_AUTOSIZE);
	namedWindow(result_window, CV_WINDOW_AUTOSIZE);

	for (;;) {
		capture.grab();
		capture.retrieve(frame);
		imshow(frame_window, frame);

		//cvtColor(frame, gray, CV_RGB2GRAY);
		//threshold(gray, binary, 10, 255, THRESH_OTSU);
		//cvtColor(binary, binary, CV_GRAY2RGB);

		scene = frame;

		int c = waitKey(30);
		if ((char) c == 'q') {
			break;
		} else if ((char) c == 'x') {
			threshold += 0.01;
			cout << "Threshold +0.01 = " << threshold << endl;
		} else if ((char) c == 'z') {
			threshold -= 0.01;
			cout << "Threshold -0.01 = " << threshold << endl;
		} else if ((char) c == '1') {
			match_method = 1;
			cout << "match_method = SQDIFF (" << match_method << ")" << endl;
		} else if ((char) c == '2') {
			match_method = 2;
			cout << "match_method = SQDIFF NORMED (" << match_method << ")"
					<< endl;
		} else if ((char) c == '3') {
			match_method = 3;
			cout << "match_method = TM CCORR (" << match_method << ")" << endl;
		} else if ((char) c == '4') {
			match_method = 4;
			cout << "match_method = TM COEFF (" << match_method << ")" << endl;
		} else if ((char) c == '5') {
			match_method = 5;
			cout << "match_method = TM COEFF NORMED (" << match_method << ")"
					<< endl;
		} else if ((char) c == 'r') {

			scene.copyTo(img_display);

			int result_cols = scene.cols - templ.cols + 1;
			int result_rows = scene.rows - templ.rows + 1;
			result.create(result_cols, result_rows, CV_32FC1);

			matchTemplate(scene, templ, result, match_method);
			normalize(result, result_norm, 0, 1, NORM_MINMAX, -1, Mat());

			double minVal;
			double maxVal;
			Point minLoc;
			Point maxLoc;
			Point matchLoc;

			vector<Point> matched;

			minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
			//matchedPoints.push_back(maxLoc);
			cout << "max = " << result.at<float>(maxLoc) << endl;

			if (match_method == CV_TM_SQDIFF
					|| match_method == CV_TM_SQDIFF_NORMED) {
				matched = getMatches(result, threshold, false);
			} else {
				matched = getMatches(result, threshold, true);
			}

			for (vector<Point>::iterator it = matched.begin();
					it != matched.end(); ++it) {
				rectangle(img_display, *it,
						Point(it->x + templ.cols, it->y + templ.rows),
						Scalar(0, 0, 255));
				rectangle(result_norm, *it,
						Point(it->x + templ.cols, it->y + templ.rows),
						Scalar(0, 0, 255));
			}

			imshow(image_window, img_display);
			imshow(result_window, result_norm);
		}
	}
	return 0;
}

vector<Point> getMatches(Mat I, float thresh, bool max) {

	assert(I.channels() == 1);

	vector<Point> matchPoints;

	for (int i = 0; i < I.rows; i++) {
		for (int j = 0; j < I.cols; j++) {
			if (max) {
				if (I.at<float>(i, j) >= thresh) {
					matchPoints.push_back(Point(j, i));
				}
			} else {
				if (I.at<float>(i, j) <= thresh) {
					matchPoints.push_back(Point(j, i));
				}
			}
		}
	}

	return matchPoints;
}
