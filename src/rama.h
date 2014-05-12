#ifndef RAMA_H
#define RAMA_H

#include "ofMain.h"
#include "patternrecognizer.h"
#include "ofxGui.h"
#include "constants.h"

namespace Ramayana {

struct Block;
struct CollidableObjects;

enum class RamaState {
  IDLE = 0,
  WALKING,
  JUMPING,
  FIRING,
  FIRED
};

enum class ArrowType {
  NORMAL,
  BRAHMASTRA
};

enum class ArrowState {
  AIRBORNE,
  GROUNDED
};

struct Arrow {
  ArrowType type;
  ArrowState state;

  ofPoint position;
  ofImage *image;
  ofVec2f velocity;

  void draw(ofRectangle &bounds, ofPoint &bottomLeft);
  void update(TimeMillis &timeElapsed);
};

class Rama {
public:
  Rama(ofPoint initialPosition);
  ~Rama();
  void draw(TimeMillis &timeElapsed, ofRectangle &bounds, ofPoint &bottomLeft);
  void update(bool *movesForFrame, CollidableObjects &collidables,
              TimeMillis &timeElapsed);

  ofPoint position;

private:
  static constexpr int maxHealth = 100;
  static constexpr long timeInFiringState = 500;
  static constexpr long timeInFiredState = 250;

  void fireArrow();

  void updateFiring(bool *moves, CollidableObjects &collidables,
                  TimeMillis &timeElapsed);
  void updateIdle(bool *moves, CollidableObjects &collidables,
                  TimeMillis &timeElapsed);
  void updateWalking(bool *moves, CollidableObjects &collidables,
                     TimeMillis &timeElapsed);
  void updateJumping(bool *moves, CollidableObjects &collidables,
                     TimeMillis &timeElapsed);
  void updateFired(bool *moves, CollidableObjects &collidables,
                  TimeMillis &timeElapsed);

  void drawFired(TimeMillis &timeElapsed, ofRectangle &bounds,
                ofPoint &bottomLeft);
  void drawFiring(TimeMillis &timeElapsed, ofRectangle &bounds,
                ofPoint &bottomLeft);
  void drawIdle(TimeMillis &timeElapsed, ofRectangle &bounds,
                ofPoint &bottomLeft);
  void drawWalking(TimeMillis &timeElapsed, ofRectangle &bounds,
                   ofPoint &bottomLeft);
  void drawJumping(TimeMillis &timeElapsed, ofRectangle &bounds,
                   ofPoint &bottomLeft);

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
  ofImage ramaFiring;
  ofImage ramaFired;

  ofImage defaultArrowImage;

  double speed;
  bool onBlock;
  Block *weightBearingBlock;

  ofVec2f velocity;
  ofxPanel gui;
  ofxLabel positionX;
  ofxLabel positionY;
  ofxLabel velocityX;
  ofxLabel velocityY;

  ofxLabel debug;

  ofxGuiGroup characterHud;
  int health;
  ofxIntSlider healthIndicator;

  long stateTimer;

  std::vector<Arrow> arrowsInFlight;
};

}

#endif
