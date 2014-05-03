#include "patternrecognizer.h"
#include "colorfilters.h"
#include "findconnected.h"
#include "matrixops.h"
#include "constants.h"

using namespace std;
using namespace Ramayana;
using namespace MatrixOperations;

RednessFilter rednessFilter;

void PlayerMovementRecognizer::draw() {
  rednessFilterImage.draw(bounds);
  gui.draw();
}

void PlayerMovementRecognizer::configure(const ofRectangle &bounds) {
  gui.setup();
  gui.add(centerOfMassLabelX.setup("X", "COM"));
  gui.add(centerOfMassLabelY.setup("Y", "COM"));
  gui.setPosition(bounds.x, bounds.y);
  this->bounds = bounds;
}

vector<InputAction>
PlayerMovementRecognizer::provideActions(cv::Mat &sourceImage) {
  vector<InputAction> actionsForFrame;

  cv::Mat newMat = applyPixelFilter(sourceImage, rednessFilter);

  int maxArea;
  cv::Point maxCenterOfMass;

  cv::Mat contourMat = drawMaxCountour(newMat, maxArea, maxCenterOfMass);
  centerOfMassLabelX.setup("X=", to_string(maxCenterOfMass.x));
  centerOfMassLabelY.setup("Y=", to_string(maxCenterOfMass.y));

  if (maxCenterOfMass.x > (CAPTURE_WIDTH * 2 / 3)) {
    actionsForFrame.push_back(Ramayana::MOVE_RIGHT);
  }

  rednessFilterImage.setFromPixels(contourMat.data, contourMat.cols,
                                   contourMat.rows, OF_IMAGE_COLOR);

  return actionsForFrame;
}
