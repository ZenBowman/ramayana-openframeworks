#ifndef FIND_CONNECTED_H
#define FIND_CONNECTED_H

#include "ofxOpenCv.h"

cv::Mat drawMaxCountour(cv::Mat &src, int &maxAreaOut,
                        cv::Point &maxCenterOfMassOut, int minArea);

#endif
