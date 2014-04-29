#include "ofApp.h"
#include "matrixops.h"
#include "ofxOpenCv.h"
#include "colorfilters.h"
#include "findconnected.h"

MatrixOperations::RednessFilter rednessFilter;
MatrixOperations::BluenessFilter bluenessFilter;

//--------------------------------------------------------------
void ofApp::setup(){
    gui.setup();
    gui.add(colorDominationFactorSlider.setup( "Ratio", 1.2f, 1.0f, 3.0f ));
    gui.setPosition(600,0);
    colorDominationFactorSlider.addListener(this, &ofApp::colorDominationFactorChanged);
    cam.initGrabber(CAPTURE_WIDTH, CAPTURE_HEIGHT, false);
}

void ofApp::colorDominationFactorChanged(float &cdf){
	rednessFilter.colorDominationFactor = cdf;
}


//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if (cam.isFrameNew()) {
        background.setFromPixels(cam.getPixels(), cam.getWidth(), cam.getHeight(), OF_IMAGE_COLOR);
        backgroundImg.setFromPixels(background);
        ofxCvColorImage frameImg;
        frameImg.setFromPixels(background);

        cv::Mat frameMat(frameImg.getCvImage());
        cv::cvtColor(frameMat, frameMat, CV_RGB2BGR);
        ofLog(OF_LOG_NOTICE, "Frameimage, width = %d, height = %d", frameMat.cols, frameMat.rows);
        cv::Mat newMat = MatrixOperations::applyPixelFilter(frameMat, rednessFilter);
        cv::Mat contourMat = drawMaxCountour(newMat);
        //rednessFilterImage.setFromPixels(newMat.data, newMat.cols, newMat.rows, OF_IMAGE_GRAYSCALE);
        rednessFilterImage.setFromPixels(contourMat.data, contourMat.cols, contourMat.rows, OF_IMAGE_COLOR);
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    backgroundImg.draw(0, 0, 300, 200);
    rednessFilterImage.draw(300, 0, 300, 200);
    gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
