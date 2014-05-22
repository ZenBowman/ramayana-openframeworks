#ifndef SOUND_RECOGNIZER_H
#define SOUND_RECOGNIZER_H

#include <vector>
#include <complex>
#include <fftw3.h>
#include "ofMain.h"
#include "ofxGui.h"
#include "patternrecognizer.h"
#include "zmq.hpp"

class AsrThread: public ofThread {
public:
  void threadedFunction() override;
  std::string lastMessage;
  std::vector<Ramayana::InputAction> queuedActions;
};

class SoundRecognizer {
public:
  void audioIn(float *input, int bufferSize, int nChannels);
  void draw();
  void update();
  vector<Ramayana::InputAction> provideActions();

  SoundRecognizer(ofPoint subWindowSize);
  ~SoundRecognizer();

private:
  bool initializedPocketSphinx;
  static constexpr size_t bufferSize = 512;
  float left[bufferSize];
  float right[bufferSize];

  fftw_complex fftIn[bufferSize];
  fftw_complex fftOut[bufferSize];
  short audioForRecognition[bufferSize];
  unsigned int count;
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
  AsrThread recvThread;
};

#endif
