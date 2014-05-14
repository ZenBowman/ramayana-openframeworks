#include "rakshas.h"
#include "rama.h"

const int RAKSHAS_WIDTH = 100;
const int RAKSHAS_HEIGHT = 150;

using namespace Ramayana;

Rakshas::Rakshas(ofPoint _initialPosition, ofPoint _finalPosition, long _period)
    : position(_initialPosition), initialPosition(_initialPosition),
      finalPosition(_finalPosition), period(_period),
      state(RakshasState::MOVING_TOWARDS_FINAL), periodTimeElapsed(0.0), alive(true) {
  velocity.x = static_cast<float>(finalPosition.x - initialPosition.x) / static_cast<float>(period);
  velocity.y = static_cast<float>(finalPosition.y - initialPosition.y) / static_cast<float>(period);

  rakshasLeft.loadImage("rakshasLeft.png");
  rakshasRight.loadImage("rakshasRight.png");
}

Rakshas::~Rakshas() {}

bool Rakshas::isAlive() {
  return alive;
}

void Rakshas::update(TimeMillis &timeElapsed) {
    if (!alive) {
      return;
  }
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

ofRectangle Rakshas::getBounds() {
  return ofRectangle(position.x + 10, position.y - 10, RAKSHAS_WIDTH - 20,
                     RAKSHAS_HEIGHT - 20);
}

void Rakshas::arrowStrike(Arrow &arrow) {
  alive = false;
}

void Rakshas::draw(ofRectangle &bounds, ofPoint &bottomLeft) {
  if (!alive) {
      return;
  }
  ofImage *imageToDraw;
  if (velocity.x <= 0) {
    imageToDraw = &rakshasLeft;
  } else {
    imageToDraw = &rakshasRight;
  }
  imageToDraw->draw(position.x - bottomLeft.x,
                    bounds.height - RAKSHAS_HEIGHT - position.y + bottomLeft.y,
                    RAKSHAS_WIDTH, RAKSHAS_HEIGHT);

}
