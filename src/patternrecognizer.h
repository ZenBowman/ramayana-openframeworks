#ifndef PATTERN_RECOGNIZER_H
#define PATTERN_RECOGNIZER_H

#include "ofxOpenCv.h"
#include "ofxGui.h"

namespace Ramayana {

enum InputAction {
  MOVE_RIGHT = 0,
  MOVE_LEFT,
  JUMP,

  NUM_ACTIONS
};

class PatternRecognizer {
public:
  virtual void draw() = 0;
  virtual void configure(const ofRectangle &bounds) = 0;
  virtual std::vector<InputAction> provideActions(cv::Mat &sourceImage) = 0;
};

struct MoveLine {
  int x1;
  int x2;
  int y1;
  int y2;

  void draw();
};

// Recognizes the biggest red object in the scene and uses
// it to control left/right movement and jumping
class PlayerMovementRecognizer : PatternRecognizer
{
public:
  void draw();
  void configure(const ofRectangle &bounds) override;
  std::vector<InputAction> provideActions(cv::Mat &sourceImage) override;
private:
  static constexpr int centerOfMassHistorySize = 7;
  static constexpr double smoothedContributionFactor = 1/ (double) centerOfMassHistorySize;

  ofRectangle bounds;
  ofxPanel gui;
  ofImage rednessFilterImage;
  ofxLabel areaLabel;
  ofxLabel centerOfMassLabelX;
  ofxLabel centerOfMassLabelY;
  double area;
  cv::Point maxCenterOfMass;
  cv::Point smoothedCenterOfMass;
  cv::Point centerOfMassHistory[centerOfMassHistorySize];

  double xScale;
  double yScale;

  MoveLine rightMoveLine;
  MoveLine jumpLine;
};


}

#endif
