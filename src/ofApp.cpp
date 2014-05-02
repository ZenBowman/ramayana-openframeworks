#include "ofApp.h"
#include "matrixops.h"
#include "ofxOpenCv.h"
#include "colorfilters.h"
#include "findconnected.h"

MatrixOperations::RednessFilter rednessFilter;
MatrixOperations::BluenessFilter bluenessFilter;

const int SUBWINDOW_SIZE_X = 200;
const int SUBWINDOW_SIZE_Y = 150;

//--------------------------------------------------------------
void ofApp::setup() {
  subWindowSize.x = SUBWINDOW_SIZE_X;
  subWindowSize.y = SUBWINDOW_SIZE_Y;

  auto windowSize = ofGetWindowSize();

  gui.setup();
  gui.add(colorDominationFactorSlider.setup("Ratio", 1.2f, 1.0f, 3.0f));
  gui.add(centerOfMassLabelX.setup("Center of mass X", "COM"));
  gui.add(centerOfMassLabelY.setup("Center of mass Y", "COM"));
  gui.setPosition(subWindowSize.x, 0);
  colorDominationFactorSlider.addListener(this,
                                          &ofApp::colorDominationFactorChanged);
  cam.initGrabber(CAPTURE_WIDTH, CAPTURE_HEIGHT, false);

  auto playerInitialPosition = ofPoint(0, 0, 0);
  auto gameBounds = ofRectangle(0, SUBWINDOW_SIZE_Y, windowSize.x,
                                windowSize.y - SUBWINDOW_SIZE_Y);
  game = std::unique_ptr<Ramayana::Game>(
      new Ramayana::Game(playerInitialPosition, gameBounds));
}

void ofApp::colorDominationFactorChanged(float &cdf) {
  rednessFilter.colorDominationFactor = cdf;
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
    cv::Mat newMat =
        MatrixOperations::applyPixelFilter(frameMat, rednessFilter);

    int maxArea;
    cv::Point maxCenterOfMass;

    cv::Mat contourMat = drawMaxCountour(newMat, maxArea, maxCenterOfMass);
    ofLog(OF_LOG_NOTICE, "Center of mass x = %d, y = %d", maxCenterOfMass.x,
          maxCenterOfMass.y);
    centerOfMassLabelX.setup("Center of mass X=",
                             std::to_string(maxCenterOfMass.x));
    centerOfMassLabelY.setup("Center of mass Y=",
                             std::to_string(maxCenterOfMass.y));

    if (maxCenterOfMass.x > (CAPTURE_WIDTH * 2 / 3)) {
      actionsForFrame.push_back(Ramayana::MOVE_RIGHT);
    }
    //rednessFilterImage.setFromPixels(newMat.data, newMat.cols, newMat.rows,
    //OF_IMAGE_GRAYSCALE);
    rednessFilterImage.setFromPixels(contourMat.data, contourMat.cols,
                                     contourMat.rows, OF_IMAGE_COLOR);
  }

  game->update(actionsForFrame, ofGetElapsedTimeMillis());
}

//--------------------------------------------------------------
void ofApp::draw() {
  const long long timeElapsed = ofGetElapsedTimeMillis();
  ofSetColor(255);
  backgroundImg.draw(0, 0, subWindowSize.x, subWindowSize.y);
  rednessFilterImage.draw(subWindowSize.x, 0, subWindowSize.x, subWindowSize.y);
  gui.draw();
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
