//
//  matrixops.cpp
//  SimpleGame
//
//  Created by Prasan Samtani on 4/26/14.
//
//

#include "matrixops.h"

namespace MatrixOperations {

ImageMatrix applyPixelFilter(ImageMatrix &source, PixelFilter &pixelFilter) {
  ImageMatrix hsvSource;
  cvtColor(source, hsvSource, CV_BGR2HSV);
  ImageMatrix newMatrix = ImageMatrix::zeros(source.rows, source.cols, CV_8UC1);
  const int rows = source.rows;
  const int cols = source.cols;

  const unsigned char *srcRowPointer;
  const unsigned char *hsvSrcRowPointer;
  unsigned char *destRowPointer;

  // Loop variables
  Pixel pixel;
  int srcColIndex;

  for (int i = 0; i < rows; i++) {
    srcRowPointer = source.ptr<uchar>(i);
    hsvSrcRowPointer = hsvSource.ptr<uchar>(i);
    destRowPointer = newMatrix.ptr<uchar>(i);

    for (int j = 0; j < cols; j++) {
      srcColIndex = j * 3;
      pixel.blue = srcRowPointer[srcColIndex];
      pixel.green = srcRowPointer[srcColIndex + 1];
      pixel.red = srcRowPointer[srcColIndex + 2];
      pixel.hue = hsvSrcRowPointer[srcColIndex];
      pixel.saturation = hsvSrcRowPointer[srcColIndex + 1];
      pixel.value = hsvSrcRowPointer[srcColIndex + 2];

      if (pixelFilter.passesFilter(pixel)) {
        destRowPointer[j] = 255;
      }
    }
  }

  return newMatrix;
}

bool doesCollide(const ofRectangle &r1, const ofRectangle &r2) {
  return (!((r2.x > (r1.x + r1.width)) || ((r2.x + r2.width) < r1.x) ||
        ((r2.y + r2.height) < r1.y) || (r2.y > (r1.y + r1.height))));
}


RelativeDirection getRelativePosition(const ofRectangle &r1, const ofRectangle &r2) {
  RelativeDirection direction;
  if (r1.x > r2.x) {
    direction.isToTheRightOf = true;
  }
  if (r1.y > r2.y) {
    direction.isAbove = true;
  }
  return direction;
}

}
