#ifndef SOUND_RECOGNIZER_H
#define SOUND_RECOGNIZER_H

#include <vector>
#include <complex>
#include <fftw3.h>
#include "ofMain.h"
#include "ofxGui.h"
#include "patternrecognizer.h"

class SoundRecognizer {
public:
  void audioIn(float *input, int bufferSize, int nChannels);
  void draw();
  void update();
  vector<Ramayana::InputAction> provideActions();

  SoundRecognizer(ofPoint subWindowSize);
  ~SoundRecognizer();

private:
  static constexpr size_t bufferSize = 512;
  float left[bufferSize];
  float right[bufferSize];

  fftw_complex fftIn[bufferSize];
  fftw_complex fftOut[bufferSize];

  std::vector<float> soundBuffer;
  ofPoint subWindowSize;
  fftw_plan fftPlan;

  ofxGuiGroup frequencies;
  ofxLabel lowFrequencyTotal;
  ofxLabel highFrequencyTotal;
  ofxLabel minFrequency;
  ofxLabel maxFrequency;

  double lowFrequencyPower;
  double highFrequencyPower;

  bool shootTriggered;
};

#endif
