#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include "filter.h"
#include "ofxOpenCv.h"

using namespace std;

typedef cv::Mat ImageMatrix;

namespace MatrixOperations {

ImageMatrix applyPixelFilter(ImageMatrix &source, PixelFilter &pixelFilter);

}

#endif
