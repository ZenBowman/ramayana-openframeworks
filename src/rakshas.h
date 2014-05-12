#ifndef RAKSHAS_H
#define RAKSHAS_H

#include "ofMain.h"
#include "constants.h"

enum class RakshasState {
  MOVING_TOWARDS_FINAL, MOVING_BACK_TOWARDS_INITIAL
};

class Rakshas {
public:
  void update(TimeMillis &timeElapsed);
  void draw(ofRectangle &bounds, ofPoint &bottomLeft);
  ofRectangle getBounds();

  // Initialize with an initial and final position, the Rakshas will interpolate
  // between those positions with the given period
  Rakshas(ofPoint initialPosition, ofPoint finalPosition, long period);
  ~Rakshas();

private:
  ofPoint position;
  ofPoint initialPosition;
  ofPoint finalPosition;
  long period;
  ofVec2f velocity;
  RakshasState state;

  ofImage rakshasLeft;
  ofImage rakshasRight;
  long periodTimeElapsed;
};

typedef std::vector<Rakshas> RakshasVect;

#endif
