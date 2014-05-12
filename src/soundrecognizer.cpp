#include "soundrecognizer.h"

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
