#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"
#include "game.h"
#include "patternrecognizer.h"
#include "constants.h"

enum KeyMap {
    KEY_RIGHT = 0,
    KEY_UP = 1
};

class ofApp : public ofBaseApp {
public:
  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
  void colorDominationFactorChanged(float &cdf);
  void audioIn(float * input, int bufferSize, int nChannels) override;

private:
  ofSoundStream soundStream;
  long long lastElapsedTime;

  ofVideoGrabber cam;
  ofPixels background;
  ofImage backgroundImg;
  ofImage rednessFilterImage;
  cv::Mat backgroundMat;
  ofxPanel gui;
  ofxFloatSlider colorDominationFactorSlider;


  ofPoint subWindowSize;
  std::unique_ptr<Ramayana::Game> game;
  std::vector<Ramayana::InputAction> actionsForFrame;

  float left[256];
  float right[256];

  bool keyDown[4];
};
