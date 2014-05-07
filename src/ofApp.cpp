#include "ofApp.h"
#include "matrixops.h"
#include "ofxOpenCv.h"
#include "colorfilters.h"
#include "portaudio.h"

Ramayana::PlayerMovementRecognizer movementRecognizer;

const int SUBWINDOW_SIZE_X = 200;
const int SUBWINDOW_SIZE_Y = 150;

void debug(std::string msg) { ofLog(OF_LOG_NOTICE, msg); }

//--------------------------------------------------------------
void ofApp::setup() {
  lastElapsedTime = 0L;
  subWindowSize.x = SUBWINDOW_SIZE_X;
  subWindowSize.y = SUBWINDOW_SIZE_Y;

  auto windowSize = ofGetWindowSize();
  cam.initGrabber(CAPTURE_WIDTH, CAPTURE_HEIGHT, false);

  auto playerInitialPosition = ofPoint(0, 20, 0);
  auto gameBounds = ofRectangle(0, SUBWINDOW_SIZE_Y, windowSize.x,
                                windowSize.y - SUBWINDOW_SIZE_Y);
  game = std::unique_ptr<Ramayana::Game>(
      new Ramayana::Game(playerInitialPosition, gameBounds));

  movementRecognizer.configure(
      ofRectangle(subWindowSize.x, 0, subWindowSize.x, subWindowSize.y));

  soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
}

void ofApp::audioIn(float *input, int bufferSize, int nChannels) {

  float curVol = 0.0;

  // samples are "interleaved"
  int numCounted = 0;

  //lets go through each sample and calculate the root mean square which is a
  //rough way to calculate volume
  for (int i = 0; i < bufferSize; i++) {
    left[i] = input[i * 2] * 0.5;
    right[i] = input[i * 2 + 1] * 0.5;

    curVol += left[i] * left[i];
    curVol += right[i] * right[i];
    numCounted += 2;
  }

  //this is how we get the mean of rms :)
  curVol /= (float) numCounted;

  // this is how we get the root of rms :)
  curVol = sqrt(curVol);

  soundBuffer.push_back(curVol);
  if (soundBuffer.size() > bufferSize) {
    soundBuffer.erase(soundBuffer.begin());
  }
}

//--------------------------------------------------------------
void ofApp::update() {
  const long long newElapsedTime = ofGetElapsedTimeMillis();
  const long long deltaTime = newElapsedTime - lastElapsedTime;

  lastElapsedTime = newElapsedTime;

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

    if (keyDown[KeyMap::KEY_RIGHT]) {
      actionsForFrame.push_back(Ramayana::InputAction::MOVE_RIGHT);
    }
    if (keyDown[KeyMap::KEY_UP]) {
      actionsForFrame.push_back(Ramayana::InputAction::JUMP);
    }
    auto actions = movementRecognizer.provideActions(frameMat);
    actionsForFrame.insert(actionsForFrame.end(), actions.begin(),
                           actions.end());
  }

  game->update(actionsForFrame, deltaTime);
}

//--------------------------------------------------------------
void ofApp::draw() {
  const long long timeElapsed = ofGetElapsedTimeMillis();
  ofSetColor(255);
  game->draw(timeElapsed);
  backgroundImg.draw(0, 0, subWindowSize.x, subWindowSize.y);
  movementRecognizer.draw();

  ofPushStyle();
  ofPushMatrix();
  ofTranslate(subWindowSize.x * 2, 0, 0);

  ofSetColor(60);
  ofDrawBitmapString("Left Channel", 4, 18);

  ofSetLineWidth(1);
  ofRect(0, 0, subWindowSize.x, subWindowSize.y);

  ofSetColor(245, 58, 135);
  ofSetLineWidth(3);

  ofBeginShape();
  const int soundBufferSize = soundBuffer.size();
  unsigned int i = 0;
  unsigned int originalI = 0;
  if (soundBufferSize > subWindowSize.x) {
    originalI = i = soundBufferSize - subWindowSize.x;
  }
  for (i; i < soundBufferSize; i++) {
    ofVertex(i - originalI, 100 - soundBuffer[i] * 500.0f);
  }
  ofEndShape(false);

  ofPopMatrix();
  ofPopStyle();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
  if (key == OF_KEY_RIGHT) {
    keyDown[KeyMap::KEY_RIGHT] = true;
  } else if (key == OF_KEY_UP) {
    keyDown[KeyMap::KEY_UP] = true;
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
  if (key == OF_KEY_RIGHT) {
    keyDown[KeyMap::KEY_RIGHT] = false;
  } else if (key == OF_KEY_UP) {
    keyDown[KeyMap::KEY_UP] = false;
  }
}

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
