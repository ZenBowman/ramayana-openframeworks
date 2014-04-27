#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

const int CAPTURE_WIDTH = 800;
const int CAPTURE_HEIGHT = 600;

class ofApp : public ofBaseApp{
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

    private:
		ofVideoGrabber cam;
		ofPixels background;
		ofImage backgroundImg;
		cv::Mat backgroundMat;
};
