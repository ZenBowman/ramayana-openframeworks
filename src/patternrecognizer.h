#ifndef PATTERN_RECOGNIZER_H
#define PATTERN_RECOGNIZER_H

#include "ofxOpenCv.h"
#include "ofxGui.h"

namespace Ramayana {

enum class InputAction {
  MOVE_RIGHT = 0,
  MOVE_LEFT,
  JUMP
};

class PatternRecognizer {
public:
  virtual void draw() = 0;
  virtual void configure(const ofRectangle &bounds) = 0;
  virtual std::vector<InputAction> provideActions(cv::Mat &sourceImage) = 0;
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
  ofRectangle bounds;
  ofxPanel gui;
  ofImage rednessFilterImage;
  ofxLabel areaLabel;
  ofxLabel centerOfMassLabelX;
  ofxLabel centerOfMassLabelY;
  double area;
};


}

#endif
