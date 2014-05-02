#include "rama.h"

#include "ofMain.h"
#include "patternrecognizer.h"

namespace Ramayana {

const int RAMA_WIDTH = 100;
const int RAMA_HEIGHT = 100;

Rama::Rama(ofPoint initialPosition) {
  position = initialPosition;
  ramaIdle.loadImage("ramaIdle.png");
  ramaWalk1.loadImage("ramaWalk1.png");
  ramaWalk2.loadImage("ramaWalk2.png");
  ramaWalk3.loadImage("ramaWalk3.png");
  ramaWalk4.loadImage("ramaWalk4.png");
}

Rama::~Rama() {}

void Rama::update(std::vector<Ramayana::InputAction> &movesForFrame,
                  const long long &timeElapsed) {
  bool anyMovement = false;
  for (const auto action : movesForFrame) {
    if (action == MOVE_RIGHT) {
      anyMovement = true;
      state = WALKING;
      position.x++;

    }
  }
  if (!anyMovement) {
    state = IDLE;
  }

}

void Rama::draw(const long long &timeElapsed, ofRectangle &bounds) {
  if (state == IDLE) {
    ramaIdle.draw(position.x,
                  bounds.y + bounds.height - RAMA_HEIGHT - position.y,
                  RAMA_WIDTH, RAMA_HEIGHT);
  } else if (state == WALKING) {
    const int timeElapsedPeriod = timeElapsed % 600;

    if (timeElapsedPeriod > 450) {
      ramaWalk1.draw(position.x,
                     bounds.y + bounds.height - RAMA_HEIGHT - position.y,
                     RAMA_WIDTH, RAMA_HEIGHT);
    } else if (timeElapsedPeriod > 300) {
      ramaWalk2.draw(position.x,
                     bounds.y + bounds.height - RAMA_HEIGHT - position.y,
                     RAMA_WIDTH, RAMA_HEIGHT);
    } else if (timeElapsedPeriod > 150) {
      ramaWalk3.draw(position.x,
                     bounds.y + bounds.height - RAMA_HEIGHT - position.y,
                     RAMA_WIDTH, RAMA_HEIGHT);
    } else {
      ramaWalk4.draw(position.x,
                     bounds.y + bounds.height - RAMA_HEIGHT - position.y,
                     RAMA_WIDTH, RAMA_HEIGHT);
    }
  }
}

}
