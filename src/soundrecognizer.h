#ifndef SOUND_RECOGNIZER_H
#define SOUND_RECOGNIZER_H

#include <vector>
#include <complex.h>
#include <fftw3.h>

class SoundRecognizer {
public:
  void audioIn(float *input, int bufferSize, int nChannels);
  void draw();

private:
  static constexpr size_t bufferSize = 512;
  float left[bufferSize];
  float right[bufferSize];

  fftw_complex fftIn[bufferSize];
  fftw_complex fftOut[bufferSize];

  std::vector<float> soundBuffer;

};

#endif
