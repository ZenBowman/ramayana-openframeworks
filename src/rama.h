#ifndef RAMA_H
#define RAMA_H

#include "ofMain.h"
#include "patternrecognizer.h"
#include "ofxGui.h"

namespace Ramayana {

struct Block;

enum class RamaState {
  IDLE = 0, WALKING, JUMPING
};

class Rama;

typedef std::vector<Block> BlockVect;
typedef const long long TimeMillis;

class Rama {
public:
  Rama(ofPoint initialPosition);
  ~Rama();
  void draw(TimeMillis &timeElapsed, ofRectangle &bounds, ofPoint &bottomLeft);
  void update(bool *movesForFrame, BlockVect &blocks, TimeMillis &timeElapsed);

  ofPoint position;

private:

  void updateIdle(bool *moves, BlockVect &blocks, TimeMillis &timeElapsed);
  void updateWalking(bool *moves, BlockVect &blocks, TimeMillis &timeElapsed);
  void updateJumping(bool *moves, BlockVect &blocks, TimeMillis &timeElapsed);


  void drawIdle(TimeMillis &timeElapsed, ofRectangle &bounds, ofPoint &bottomLeft);
  void drawWalking(TimeMillis &timeElapsed, ofRectangle &bounds, ofPoint &bottomLeft);
  void drawJumping(TimeMillis &timeElapsed, ofRectangle &bounds, ofPoint &bottomLeft);

  void drawRama(ofImage &image, ofRectangle &bounds, ofPoint &bottomLeft);

  friend class IdleStateBehavior;

  RamaState state;

  ofImage ramaIdle;
  ofImage ramaWalk1;
  ofImage ramaWalk2;
  ofImage ramaWalk3;
  ofImage ramaWalk4;
  ofImage ramaWalk5;
  ofImage ramaWalk6;
  ofImage ramaJumping;

  bool onBlock;
  Block *weightBearingBlock;

  double speed;
  ofVec2f velocity;
  ofxPanel gui;
  ofxLabel positionX;
  ofxLabel positionY;
  ofxLabel velocityX;
  ofxLabel velocityY;

  ofxLabel debug;
};

}

#endif
