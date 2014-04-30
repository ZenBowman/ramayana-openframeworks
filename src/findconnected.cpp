#include "ofxOpenCv.h"
#include <vector>

using namespace std;
using namespace cv;

RNG rng(12345);

Mat drawMaxCountour(Mat &src, int &maxAreaOut, cv::Point &maxCenterOfMassOut) {
    Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( src, contours, hierarchy,
        CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    maxAreaOut = 0;
    int maxAreaIndex = 0;
    int currentArea;


    for (int i=0; i<contours.size(); i++) {
        auto contour = contours[i];
        currentArea = contourArea(contour);

        if (currentArea > maxAreaOut) {
            maxAreaOut = currentArea;
            auto m = moments(contour);
            maxCenterOfMassOut.x = m.m10/m.m00;
            maxCenterOfMassOut.y = m.m01/m.m00;
            //ofLog(OF_LOG_NOTICE, "Moments = m10=%d, m01=%d, m00=%d", m.m10, m.m01, m.m00);
            maxAreaIndex = i;
        }
    }

    Scalar color = Scalar(255, 0, 0);
    drawContours( dst, contours, maxAreaIndex, color, CV_FILLED, 8, hierarchy, 0, Point() );


    return dst;
}
