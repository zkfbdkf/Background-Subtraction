#pragma warning (disable:4996)
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

vector <vector<Point>> contours;
vector<Vec4i> hierarchy;


int main(){

	VideoCapture capture("background.mp4");
	bool stop(false);
	int key = 0;
	double rate = capture.get(CV_CAP_PROP_FPS);
	int delay = 1000 / rate;

	Mat frame,original, first,subtraction,gray;
	capture >> first;

	Mat element = getStructuringElement(MORPH_RECT, Size(4, 10));
	Mat element2 = getStructuringElement(MORPH_RECT, Size(2, 10));
	Mat element3 = getStructuringElement(MORPH_RECT, Size(3, 7));

	char msg[100];


	Point pt1;

	while (capture.read(frame)){
		if (!stop){
			absdiff(frame, first, subtraction);

			cvtColor(subtraction, gray, CV_BGR2GRAY);

			threshold(gray, gray, 50, 255, THRESH_BINARY);
			medianBlur(gray, gray, 7);

			morphologyEx(gray, gray, CV_MOP_CLOSE, element);//닫기
			morphologyEx(gray, gray, CV_MOP_CLOSE, element);//닫기
			dilate(gray, gray, element2);
			dilate(gray, gray, element2);
			dilate(gray, gray, element2);
			dilate(gray, gray, element2);
			dilate(gray, gray, element2);
			dilate(gray, gray, element3);


			Mat copy = gray.clone();
			int cnt = 1;
			findContours(copy, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

			for (int i = 0; i < contours.size(); i++) {
				//drawContours(frame, contours, i, Scalar(0, 255, 0), CV_FILLED, 8, hierarchy); // 세 번째 인자인 i는 몇 번째 contour를 그릴 것인가
				Rect rect = boundingRect(contours[i]);
				if (rect.area() > 1850){
					rectangle(frame, rect.tl(), rect.br(), Scalar(0, 255, 0), 2, 8, 0);  // 첫 번째 인자는 어느 Mat에 그릴 것인가, 두 번째와 세 번째 인자는 직사각형의 왼쪽 가장 위 꼭지점과 오른쪽 가장 아래 꼭지점을 나타내는 것(top left와 bottom right), 2는 사각형의 두께, 라인의 형태

					pt1 = rect.tl();

					sprintf(msg, "%d", cnt);
					putText(frame, msg, pt1, FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255));
					cnt++;
				}
			}

			sprintf(msg, "number of people = %d", cnt-1);
			putText(frame, msg, Point(20, 40), FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 0, 255));

			imshow("frame", frame);
			//imshow("sub", subtraction);
			imshow("gray", gray);

			key = waitKey(1);
			if (key == 32){
				if (stop == false)
					stop = true;
				else stop = false;
			}
			else if (key == 27)
				break;
		}
	}

}
