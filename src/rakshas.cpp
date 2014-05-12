#include "rakshas.h"

const int RAKSHAS_WIDTH = 100;
const int RAKSHAS_HEIGHT = 150;

Rakshas::Rakshas(ofPoint _initialPosition, ofPoint _finalPosition, long _period)
    : position(_initialPosition),
      initialPosition(_initialPosition),
      finalPosition(_finalPosition),
      period(_period),
     state(RakshasState::MOVING_TOWARDS_FINAL),
      periodTimeElapsed(0.0) {
  velocity.x = (finalPosition.x - initialPosition.x) / period;
  velocity.y = (finalPosition.y - initialPosition.y) / period;

  rakshasLeft.loadImage("rakshas1.png");
}

Rakshas::~Rakshas() {}

void Rakshas::update(TimeMillis &timeElapsed) {
  position.x += velocity.x * timeElapsed;
  position.y += velocity.y * timeElapsed;
  periodTimeElapsed += timeElapsed;

  if (periodTimeElapsed > period) {
    periodTimeElapsed = 0;
    if (state == RakshasState::MOVING_TOWARDS_FINAL) {
      state = RakshasState::MOVING_BACK_TOWARDS_INITIAL;
      velocity.x = -velocity.x;
      velocity.y = -velocity.y;
    } else {
      state = RakshasState::MOVING_TOWARDS_FINAL;
      velocity.x = -velocity.x;
      velocity.y = -velocity.y;
    }
  }
}

void Rakshas::draw(ofRectangle &bounds, ofPoint &bottomLeft)
{
    rakshasLeft.draw(position.x - bottomLeft.x,
             bounds.height - RAKSHAS_HEIGHT - position.y - bottomLeft.y,
             RAKSHAS_WIDTH, RAKSHAS_HEIGHT);
   }
