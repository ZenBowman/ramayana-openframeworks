#ifndef RAMA_H
#define RAMA_H

#include "ofMain.h"
#include "patternrecognizer.h"

namespace Ramayana {
enum RamaState {
  IDLE = 0,
  WALKING
};

class Rama {
public:
  Rama(ofPoint initialPosition);
  ~Rama();
  void draw(const long long &timeElapsed, ofRectangle &bounds,
            ofPoint &bottomLeft);
  void update(std::vector<Ramayana::InputAction> &movesForFrame,
              const long long &timeElapsed);

  ofPoint position;

private:
  RamaState state;

  ofImage ramaIdle;
  ofImage ramaWalk1;
  ofImage ramaWalk2;
  ofImage ramaWalk3;
  ofImage ramaWalk4;


};

}

#endif