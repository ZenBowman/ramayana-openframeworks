#include "soundrecognizer.h"
#include "ofMain.h"
#include <complex>

using namespace Ramayana;

SoundRecognizer::SoundRecognizer(ofPoint _subWindowSize)
    : subWindowSize(_subWindowSize), shootTriggered(false) {
  fftPlan =
      fftw_plan_dft_1d(bufferSize, fftIn, fftOut, FFTW_FORWARD, FFTW_ESTIMATE);

  frequencies.setup();
  frequencies.add(lowFrequencyTotal.setup("LF:", std::to_string(0)));
  frequencies.add(highFrequencyTotal.setup("HF:", std::to_string(0)));
  frequencies.setPosition(subWindowSize.x * 2, 10);
}

SoundRecognizer::~SoundRecognizer() { fftw_destroy_plan(fftPlan); }

vector<InputAction>
SoundRecognizer::provideActions() {
  vector<InputAction> actionsForFrame;
  if (shootTriggered) {
    shootTriggered = false;
    actionsForFrame.push_back(InputAction::FIRE);
  }
  return actionsForFrame;
}

void SoundRecognizer::update() {
  constexpr unsigned int maxLowFrequencyThreshold = 50;
  constexpr unsigned int minHighFrequencyThreshold = 50;

  // insert elements into fftIn here
  fftw_execute(fftPlan);
  // extract elements from fftOut here
  lowFrequencyPower = 0;
  highFrequencyPower = 0;
  double minFreq = 9999;
  double maxFreq = 0;

  size_t maxFrequencyIndex = 0;
  size_t minFrequencyIndex = 0;

  for (size_t i = 0; i < bufferSize/2; i++) {
    std::complex<double> fftOutI(fftOut[i][0], fftOut[i][1]);
    double amount = std::abs<double>(fftOutI);

    if (amount > maxFreq) {
      maxFreq = amount;
      maxFrequencyIndex = i;
    }
    if (amount < minFreq) {
      minFreq = amount;
      minFrequencyIndex = i;
    }

    if (i > minHighFrequencyThreshold) {
      highFrequencyPower += amount;
    }
    if (i < maxLowFrequencyThreshold) {
      lowFrequencyPower += amount;
    }
  }

  if (lowFrequencyPower > 20) {
    shootTriggered = true;
  }

  lowFrequencyTotal.setup("Low frequency:", std::to_string(lowFrequencyPower));
  highFrequencyTotal.setup("High frequency:", std::to_string(highFrequencyPower));
  minFrequency.setup("Min frequency:", std::to_string(minFrequencyIndex));
  maxFrequency.setup("Max frequency:", std::to_string(maxFrequencyIndex));
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

  const int bandWidth = 1;


  for (i = 0; i < bufferSize/2; i++) {
    std::complex<double> fftOutI(fftOut[i][0], fftOut[i][1]);
    ofRect(i * bandWidth, 200, bandWidth, - std::abs<double>(fftOutI) * 50);
  }

  ofPopMatrix();
  ofPopStyle();

  frequencies.draw();
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
