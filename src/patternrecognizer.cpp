#include "patternrecognizer.h"
#include "colorfilters.h"
#include "findconnected.h"
#include "matrixops.h"
#include "constants.h"

using namespace std;
using namespace Ramayana;
using namespace MatrixOperations;

RednessFilter rednessFilter;
const int rednessFilterMinArea = 2500;

void PlayerMovementRecognizer::draw() {
  rednessFilterImage.draw(bounds);
  gui.draw();
}

void PlayerMovementRecognizer::configure(const ofRectangle &bounds) {
  gui.setup();
  gui.add(areaLabel.setup("Area", ""));
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
  areaLabel.setup("AreaX=", to_string(area));
  centerOfMassLabelX.setup("X=", to_string(maxCenterOfMass.x));
  centerOfMassLabelY.setup("Y=", to_string(maxCenterOfMass.y));

  if (maxArea > rednessFilterMinArea) {
      ofLog(OF_LOG_NOTICE, "COM y=%d, 2/3 capture=%d", maxCenterOfMass.y, CAPTURE_HEIGHT*2/3);
      if (maxCenterOfMass.y < (CAPTURE_HEIGHT * 1 / 3)) {
        actionsForFrame.push_back(Ramayana::InputAction::JUMP);
      }
      else if (maxCenterOfMass.x > (CAPTURE_WIDTH * 2 / 3)) {
        actionsForFrame.push_back(Ramayana::InputAction::MOVE_RIGHT);
      }

  }

  area = maxArea;
  rednessFilterImage.setFromPixels(contourMat.data, contourMat.cols,
                                   contourMat.rows, OF_IMAGE_COLOR);

  return actionsForFrame;
}
