#include "ofApp.h"
#include "matrixops.h"
#include "ofxOpenCv.h"
#include "colorfilters.h"

Ramayana::PlayerMovementRecognizer movementRecognizer;

const int SUBWINDOW_SIZE_X = 200;
const int SUBWINDOW_SIZE_Y = 150;

//--------------------------------------------------------------
void ofApp::setup() {
  subWindowSize.x = SUBWINDOW_SIZE_X;
  subWindowSize.y = SUBWINDOW_SIZE_Y;

  auto windowSize = ofGetWindowSize();
  cam.initGrabber(CAPTURE_WIDTH, CAPTURE_HEIGHT, false);

  auto playerInitialPosition = ofPoint(0, 20, 0);
  auto gameBounds = ofRectangle(0, SUBWINDOW_SIZE_Y, windowSize.x,
                                windowSize.y - SUBWINDOW_SIZE_Y);
  game = std::unique_ptr<Ramayana::Game>(
      new Ramayana::Game(playerInitialPosition, gameBounds));

  movementRecognizer.configure(ofRectangle(subWindowSize.x, 0, subWindowSize.x, subWindowSize.y));
}

//--------------------------------------------------------------
void ofApp::update() {
  cam.update();

  if (cam.isFrameNew()) {
    actionsForFrame.clear();
    background.setFromPixels(cam.getPixels(), cam.getWidth(), cam.getHeight(),
                             OF_IMAGE_COLOR);
    backgroundImg.setFromPixels(background);
    ofxCvColorImage frameImg;
    frameImg.setFromPixels(background);

    cv::Mat frameMat(frameImg.getCvImage());
    cv::cvtColor(frameMat, frameMat, CV_RGB2BGR);
    ofLog(OF_LOG_NOTICE, "Frameimage, width = %d, height = %d", frameMat.cols,
          frameMat.rows);
    auto actions = movementRecognizer.provideActions(frameMat);
    actionsForFrame.insert(actionsForFrame.end(), actions.begin(), actions.end());
  }

  game->update(actionsForFrame, ofGetElapsedTimeMillis());
}

//--------------------------------------------------------------
void ofApp::draw() {
  const long long timeElapsed = ofGetElapsedTimeMillis();
  ofSetColor(255);
  backgroundImg.draw(0, 0, subWindowSize.x, subWindowSize.y);
  movementRecognizer.draw();
  game->draw(timeElapsed);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
