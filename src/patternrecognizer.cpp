#include "patternrecognizer.h"
#include "colorfilters.h"
#include "findconnected.h"
#include "matrixops.h"
#include "constants.h"

using namespace std;
using namespace Ramayana;
using namespace MatrixOperations;

RednessFilter rednessFilter(150, 1.5);
const int rednessFilterMinArea = 2500;

constexpr double moveRightMinXRatio = 5.0 / 7;
constexpr double jumpMinYRatio = 1.0 / 3;

void MoveLine::draw() { ofLine(x1, y1, x2, y2); }

void PlayerMovementRecognizer::draw() {
  rednessFilterImage.draw(bounds);
  gui.draw();
  ofSetColor(255);
  ofCircle(bounds.x + smoothedCenterOfMass.x * xScale,
           smoothedCenterOfMass.y * yScale, 5);
  rightMoveLine.draw();
  leftMoveLine.draw();
  jumpLine.draw();

  for (size_t i=0; i<faces.size(); i++) {
    ofRect(faces[i].x * xScale, faces[i].y * yScale, faces[i].width * xScale, faces[i].height * yScale);
  }
}

void PlayerMovementRecognizer::configure(const ofRectangle &bounds) {
  gui.setup();
  gui.add(areaLabel.setup("Area", ""));
  gui.add(centerOfMassLabelX.setup("X", "COM"));
  gui.add(centerOfMassLabelY.setup("Y", "COM"));
  gui.add(numFacesLabel.setup("NumFaces:", "0"));
  gui.setPosition(bounds.x, bounds.y);
  this->bounds = bounds;

  xScale = (double) bounds.width / CAPTURE_WIDTH;
  yScale = (double) bounds.height / CAPTURE_HEIGHT;

  leftMoveLine.x1 = leftMoveLine.x2 =
      bounds.x + (1 - moveRightMinXRatio) * bounds.width;
  leftMoveLine.y1 = bounds.y;
  leftMoveLine.y2 = bounds.y + bounds.height;

  rightMoveLine.x1 = rightMoveLine.x2 =
      bounds.x + moveRightMinXRatio * bounds.width;
  rightMoveLine.y1 = bounds.y;
  rightMoveLine.y2 = bounds.y + bounds.height;

  jumpLine.x1 = bounds.x;
  jumpLine.x2 = bounds.x + bounds.width;
  jumpLine.y1 = jumpLine.y2 = jumpMinYRatio * bounds.height;

  for (int i = 0; i < centerOfMassHistorySize; i++) {
    centerOfMassHistory[i] = cv::Point(0, 0);
  }

  faceCascade.load("/home/psamtani/development/opencv-2.4.7/data/haarcascades/haarcascade_frontalface_alt.xml");
}

vector<InputAction>
PlayerMovementRecognizer::provideActions(cv::Mat &sourceImage) {
  vector<InputAction> actionsForFrame;
  //cv::Mat frame_gray;
  faces.clear();

  //cv::cvtColor( sourceImage, frame_gray, CV_BGR2GRAY );
  //cv::equalizeHist( frame_gray, frame_gray );
  //cv::resize(frame_gray, frame_gray, cv::Size(SUBWINDOW_SIZE_X, SUBWINDOW_SIZE_Y));
  //-- Detect faces
  //faceCascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30) );
  numFacesLabel = std::to_string(faces.size());

  cv::Mat newMat = applyPixelFilter(sourceImage, rednessFilter);

  int maxArea;

  cv::Mat contourMat = drawMaxCountour(newMat, maxArea, maxCenterOfMass, rednessFilterMinArea);

  double smoothedCenterOfMassX = 0.0;
  double smoothedCenterOfMassY = 0.0;

  for (int i = 0; i < centerOfMassHistorySize - 1; i++) {
    centerOfMassHistory[i] = centerOfMassHistory[i + 1];
    smoothedCenterOfMassX +=
        smoothedContributionFactor * centerOfMassHistory[i].x;
    smoothedCenterOfMassY +=
        smoothedContributionFactor * centerOfMassHistory[i].y;
  }
  centerOfMassHistory[centerOfMassHistorySize - 1] = maxCenterOfMass;
  smoothedCenterOfMassX += smoothedContributionFactor * maxCenterOfMass.x;
  smoothedCenterOfMassY += smoothedContributionFactor * maxCenterOfMass.y;
  smoothedCenterOfMass.x = smoothedCenterOfMassX;
  smoothedCenterOfMass.y = smoothedCenterOfMassY;

  areaLabel.setup("AreaX=", to_string(area));
  centerOfMassLabelX.setup("X=",
                           to_string(bounds.x + maxCenterOfMass.x * xScale));
  centerOfMassLabelY.setup("Y=", to_string(maxCenterOfMass.y * yScale));

  if (maxArea > rednessFilterMinArea) {
    ofLog(OF_LOG_NOTICE, "COM y=%d, 2/3 capture=%d", maxCenterOfMass.y,
          CAPTURE_HEIGHT * 2 / 3);
    if (smoothedCenterOfMass.y < CAPTURE_HEIGHT * jumpMinYRatio) {
      actionsForFrame.push_back(Ramayana::InputAction::JUMP);
    }
    if (smoothedCenterOfMass.x > CAPTURE_WIDTH * moveRightMinXRatio) {
      actionsForFrame.push_back(Ramayana::InputAction::MOVE_RIGHT);
    }
    if (smoothedCenterOfMass.x < CAPTURE_WIDTH * (1 - moveRightMinXRatio)) {
      actionsForFrame.push_back(Ramayana::InputAction::MOVE_LEFT);
    }
  }

  area = maxArea;
  rednessFilterImage.setFromPixels(contourMat.data, contourMat.cols,
                                   contourMat.rows, OF_IMAGE_COLOR);

  return actionsForFrame;
}
