#include "soundrecognizer.h"
#include "ofMain.h"
#include <complex>

using namespace Ramayana;

void AsrThread::threadedFunction() {
  zmq::context_t context (1);
  zmq::socket_t subscriber (context, ZMQ_SUB);
  subscriber.connect("tcp://localhost:5556");
  subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

  while (isThreadRunning()) {
  zmq::message_t update;
  subscriber.recv(&update);
  const char *data = static_cast<char*>(update.data());
  lock();
  lastMessage =data;
  lastMessage = lastMessage.substr(0, update.size());
  if (lastMessage == "GO") {
    queuedActions.push_back(InputAction::FIRE);
  } else if (lastMessage == "NAMAH") {
    queuedActions.push_back(InputAction::DOUBLE_FIRE);
  }
  unlock();
  ofLogNotice() << "Received" << data;
  }
}

SoundRecognizer::SoundRecognizer(ofPoint _subWindowSize)
    : initializedPocketSphinx(false), count(0), subWindowSize(_subWindowSize),
      shootTriggered(false) {

  frequencies.setup();
  frequencies.add(lowFrequencyTotal.setup("LF:", std::to_string(0)));
  frequencies.add(highFrequencyTotal.setup("HF:", std::to_string(0)));
  frequencies.setPosition(subWindowSize.x * 2, 10);

  recvThread.startThread(true, false);
}

SoundRecognizer::~SoundRecognizer() {
  recvThread.stopThread();
}

vector<InputAction> SoundRecognizer::provideActions() {
  recvThread.lock();
  vector<InputAction> actionsForFrame;
  actionsForFrame.insert(actionsForFrame.end(), recvThread.queuedActions.begin(), recvThread.queuedActions.end());
  recvThread.queuedActions.clear();
  recvThread.unlock();
  return actionsForFrame;
}

void SoundRecognizer::update() {
  recvThread.lock();
  lowFrequencyTotal = recvThread.lastMessage;
  recvThread.unlock();
}

void SoundRecognizer::draw() {
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

  if ((count > 10) && (count < 50)) {
    //PocketSphinx::ps_process_raw(ps, audioForRecognition, 512, FALSE, FALSE);
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
