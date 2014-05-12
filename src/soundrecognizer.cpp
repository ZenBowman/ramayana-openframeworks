#include "soundrecognizer.h"
#include "ofMain.h"
#include <complex>

SoundRecognizer::SoundRecognizer(ofPoint _subWindowSize)
    : subWindowSize(_subWindowSize) {
  fftPlan =
      fftw_plan_dft_1d(bufferSize, fftIn, fftOut, FFTW_FORWARD, FFTW_ESTIMATE);
}

SoundRecognizer::~SoundRecognizer() { fftw_destroy_plan(fftPlan); }

void SoundRecognizer::update() {
  // insert elements into fftIn here
  fftw_execute(fftPlan);
  // extract elements from fftOut here
  for (int i = 0; i < bufferSize; i++) {
    ofLogNotice() << "Frequency[" << i << "] = " << fftOut[i][0] << ", "
                  << fftOut[i][1];
  }
}

void SoundRecognizer::draw() {
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
  for (; i < soundBufferSize; i++) {
    ofVertex(i - originalI, 50 - soundBuffer[i] * 800.0f);
  }
  ofEndShape(false);

  const int bandWidth = 4;

  for (i = 0; i < subWindowSize.x/bandWidth; i++) {
    std::complex<double> fftOutI(fftOut[i][0], fftOut[i][1]);
    ofRect(i * bandWidth, 200, bandWidth, - std::abs<double>(fftOutI) * 50);
  }

  ofPopMatrix();
  ofPopStyle();

}

void SoundRecognizer::audioIn(float *input, int bufferSize, int nChannels) {

  float curVol = 0.0;

  // samples are "interleaved"
  int numCounted = 0;

  //lets go through each sample and calculate the root mean square which is a
  //rough way to calculate volume
  for (int i = 0; i < bufferSize; i++) {

    left[i] = input[i * 2] * 0.5;
    right[i] = input[i * 2 + 1] * 0.5;

    fftIn[i][0] = left[i];
    fftIn[i][1] = 0.0;

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
