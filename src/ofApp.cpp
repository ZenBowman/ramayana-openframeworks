#include "ofApp.h"
#include "matrixops.h"
#include "ofxOpenCv.h"
#include "colorfilters.h"
#include "portaudio.h"

Ramayana::PlayerMovementRecognizer movementRecognizer;

void debug(std::string msg) { ofLog(OF_LOG_NOTICE, msg); }

void ofApp::resetGame() {
  auto windowSize = ofGetWindowSize();
  const ofRectangle gameBounds(0, 0, windowSize.x, windowSize.y);
  game = std::unique_ptr<Ramayana::Game>(
      new Ramayana::Game(playerInitialPosition, gameBounds));
}

//--------------------------------------------------------------
void ofApp::setup() {
  lastElapsedTime = 0L;
  subWindowSize.x = SUBWINDOW_SIZE_X;
  subWindowSize.y = SUBWINDOW_SIZE_Y;

  auto windowSize = ofGetWindowSize();
  cam.initGrabber(CAPTURE_WIDTH, CAPTURE_HEIGHT, false);

  resetGame();

  movementRecognizer.configure(
      ofRectangle(subWindowSize.x, 0, subWindowSize.x, subWindowSize.y));

  soundRecognizer = std::unique_ptr<SoundRecognizer>(new SoundRecognizer(subWindowSize));

  soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
  shoot = false;
}

void ofApp::audioIn(float *input, int bufferSize, int nChannels) {
  soundRecognizer->audioIn(input, bufferSize, nChannels);
}


//--------------------------------------------------------------
void ofApp::update() {
  const long long newElapsedTime = ofGetElapsedTimeMillis();
  const long long deltaTime = newElapsedTime - lastElapsedTime;

  lastElapsedTime = newElapsedTime;

  cam.update();
  soundRecognizer->update();

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
    if (keyDown[KeyMap::KEY_LEFT]) {
      actionsForFrame.push_back(Ramayana::InputAction::MOVE_LEFT);
    }
    if (shoot) {
      actionsForFrame.push_back(Ramayana::InputAction::FIRE);
      shoot = false;
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
  soundRecognizer->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
  if (key == OF_KEY_RIGHT) {
    keyDown[KeyMap::KEY_RIGHT] = true;
  } else if (key == OF_KEY_UP) {
    keyDown[KeyMap::KEY_UP] = true;
  } else if (key == OF_KEY_LEFT) {
    keyDown[KeyMap::KEY_LEFT] = true;
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
  if (key == OF_KEY_RIGHT) {
    keyDown[KeyMap::KEY_RIGHT] = false;
  } else if (key == OF_KEY_UP) {
    keyDown[KeyMap::KEY_UP] = false;
  } else if (key == OF_KEY_LEFT) {
    keyDown[KeyMap::KEY_LEFT] = false;
  } else if (key == ' ') {
    shoot = true;
  } else if (key == 'r') {
    resetGame();
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
