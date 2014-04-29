#include "ofxOpenCv.h"
#include <vector>

using namespace std;
using namespace cv;

RNG rng(12345);

Mat drawMaxCountour(Mat &src) {
    Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( src, contours, hierarchy,
        CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    int maxArea = 0;
    int maxAreaIndex = 0;
    int currentArea;

    for (int i=0; i<contours.size(); i++) {
        auto contour = contours[i];
        currentArea = contourArea(contour);
        if (currentArea > maxArea) {
            maxArea = currentArea;
            maxAreaIndex = i;
        }
    }

    Scalar color = Scalar(255, 0, 0);
    drawContours( dst, contours, maxAreaIndex, color, CV_FILLED, 8, hierarchy, 0, Point() );


    return dst;
}
