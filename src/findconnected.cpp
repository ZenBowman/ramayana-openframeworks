#include "ofxOpenCv.h"
#include <vector>

using namespace std;
using namespace cv;

RNG rng(12345);

Mat drawMaxCountour(Mat &src, int &maxAreaOut, cv::Point &maxCenterOfMassOut, int minArea) {
  Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  findContours(src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

  maxAreaOut = 0;
  int maxAreaIndex = 0;
  int currentArea;
  std::vector<int> indicesToDraw;
  std::vector<cv::Point> centerOfMasses;

  for (int i = 0; i < contours.size(); i++) {
    auto contour = contours[i];
    currentArea = contourArea(contour);

    if (currentArea > minArea) {
      indicesToDraw.push_back(i);
      cv::Point point;
      auto m = moments(contour);
      point.x = m.m10 / m.m00;
      point.y = m.m01 / m.m00;
      centerOfMasses.push_back(point);
    }


    if (currentArea > maxAreaOut) {
      maxAreaOut = currentArea;
      maxAreaIndex = i;
    }
  }

  if (centerOfMasses.size() == 0) {
    maxCenterOfMassOut.x = 300;
    maxCenterOfMassOut.y = 300;
  } else {
    maxCenterOfMassOut.x = 0;
    maxCenterOfMassOut.y = 0;
  }
  for (auto point : centerOfMasses) {
      maxCenterOfMassOut.x += point.x / centerOfMasses.size();
      maxCenterOfMassOut.y += point.y / centerOfMasses.size();
  }

  Scalar color = Scalar(255, 0, 0);
  for (auto i : indicesToDraw) {
  drawContours(dst, contours, i, color, CV_FILLED, 8, hierarchy, 0,
               Point());
  }

  return dst;
}
