#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include "filter.h"
#include "ofxOpenCv.h"

using namespace std;

typedef cv::Mat ImageMatrix;

namespace MatrixOperations {

struct RelativeDirection {
  RelativeDirection() : isToTheRightOf(false), isAbove(false) {}
  bool isToTheRightOf;
  bool isAbove;
};

ImageMatrix applyPixelFilter(ImageMatrix &source, PixelFilter &pixelFilter);

bool doesCollide(const ofRectangle &r1, const ofRectangle &r2);

RelativeDirection getRelativePosition(const ofRectangle &r1,
                                      const ofRectangle &r2);
}

#endif
