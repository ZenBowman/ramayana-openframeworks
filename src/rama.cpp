#include "rama.h"

#include "ofMain.h"
#include "patternrecognizer.h"
#include "matrixops.h"
#include "game.h"

using namespace MatrixOperations;

namespace Ramayana {

const int RAMA_WIDTH = 100;
const int RAMA_HEIGHT = 150;

constexpr int ARROW_WIDTH = 90;
constexpr int ARROW_HEIGHT = 12;

const int MAX_JUMP_HEIGHT = 200;
const int MIN_Y_POSITION = 20;
const double SCALE_FACTOR = 0.05;
const int JUMP_VELOCITY_X = 3;
const int JUMP_VELOCITY_Y = 20;
constexpr int IMAGE_PIXEL_ADJUSTMENT = 5;
constexpr int MAX_RUNNING_VELOCITY = 5;

Rama::Rama(ofPoint initialPosition)
    : position(initialPosition), speed(5), onBlock(false),
      weightBearingBlock(nullptr), velocity(ofVec2f(0, 0)), health(100) {
  ramaIdle.loadImage("ramaIdle.png");
  ramaWalk1.loadImage("ramaWalking1.png");
  ramaWalk2.loadImage("ramaWalking2.png");
  ramaWalk3.loadImage("ramaWalking3.png");
  ramaWalk4.loadImage("ramaWalking4.png");
  ramaWalk5.loadImage("ramaWalking5.png");
  ramaWalk6.loadImage("ramaWalking6.png");
  ramaJumping.loadImage("ramaJumping.png");
  ramaFiring.loadImage("ramaShooting1.png");
  ramaFired.loadImage("ramaShooting2.png");
  defaultArrowImage.loadImage("arrow.png");

  characterHud.setup();
  characterHud.add(velocityX.setup("Velocity.X", ""));
  characterHud.add(healthIndicator.setup("Health", health, 0, maxHealth));
  characterHud.setPosition(position);

}

Rama::~Rama() {}

ofRectangle boundingBoxFor(const ofPoint &position) {
  ofRectangle bb;
  bb.x = position.x + 10;
  bb.y = position.y - 10;
  bb.width = RAMA_WIDTH - 20;
  bb.height = RAMA_HEIGHT - 20;
  return bb;
}

void Rama::fireArrow() {
  Arrow arrow;
  arrow.type = ArrowType::NORMAL;
  arrow.state = ArrowState::AIRBORNE;
  arrow.position = position;
  arrow.position.y += static_cast<float>(RAMA_HEIGHT) / 2;
  arrow.velocity = ofVec2f(1.5f, 0.0f);
  arrow.image = &defaultArrowImage;

  arrowsInFlight.push_back(arrow);
}

void Arrow::update(TimeMillis &timeElapsed) {
  position.x += timeElapsed * velocity.x;
  position.y += timeElapsed * velocity.y;
}

void Arrow::draw(ofRectangle &bounds, ofPoint &bottomLeft) {
  image->draw(position.x - bottomLeft.x,
              bounds.height - ARROW_HEIGHT - position.y + bottomLeft.y,
              ARROW_WIDTH, ARROW_HEIGHT);
}

ofRectangle Arrow::bounds() {
  return ofRectangle(position.x, position.y, ARROW_WIDTH, ARROW_HEIGHT);
}

void Rama::updateFiring(bool *moves, CollidableObjects &collidables,
                        TimeMillis &timeElapsed) {
  stateTimer += timeElapsed;
  if (stateTimer > timeInFiringState) {
    fireArrow();
    stateTimer = 0;

    if ((--numArrowsToFire) == 0) {
      state = RamaState::FIRED;
    }

  }
}

void Rama::updateFired(bool *moves, CollidableObjects &collidables,
                       TimeMillis &timeElapsed) {
  stateTimer += timeElapsed;
  if (stateTimer > timeInFiredState) {
    state = RamaState::IDLE;
    stateTimer = 0;
  }
}

void Rama::updateJumping(bool *moves, CollidableObjects &collidables,
                         TimeMillis &timeElapsed) {
  BlockVect &blocks = collidables.blocks;
  if (state == RamaState::JUMPING) {
    position.x += velocity.x * SCALE_FACTOR * timeElapsed;
    position.y += velocity.y * SCALE_FACTOR * timeElapsed;
    velocity.y -= SCALE_FACTOR * timeElapsed; // gravity
    for (auto &block : blocks) {
      if (doesCollide(boundingBoxFor(position), block.bounds)) {
        RelativeDirection direction =
            getRelativePosition(boundingBoxFor(position), block.bounds);
        if (direction.isAbove) {
          velocity.x = 0;
          velocity.y = 0;
          position.y = block.bounds.y + block.bounds.height;
          state = RamaState::IDLE;
          onBlock = true;
          weightBearingBlock = &block;
        } else if (velocity.x < 0.1) {
          position.y = block.bounds.y - RAMA_HEIGHT;
          velocity.y = 0;
        } else {
          velocity.y = 0;
          position.x = block.bounds.x - RAMA_WIDTH;
        }

      }
    }

    if (position.y < MIN_Y_POSITION) {
      position.y = MIN_Y_POSITION;
      state = RamaState::IDLE;
      velocity.y = 0;
      velocity.x = 0;
    }
  }
}

void Rama::updateWalking(bool *moves, CollidableObjects &collidables,
                         TimeMillis &timeElapsed) {
  BlockVect &blocks = collidables.blocks;
  if (onBlock) {
    if (!doesCollide(boundingBoxFor(position), weightBearingBlock->bounds)) {
      velocity.x = 0;
      velocity.y = 0;
      state = RamaState::JUMPING;
      onBlock = false;
      weightBearingBlock = nullptr;
      return;
    }
  }
  if (moves[InputAction::JUMP]) {
    ofLog(OF_LOG_NOTICE, "Initiating jump");
    state = RamaState::JUMPING;
    if (moves[InputAction::MOVE_RIGHT]) {
      velocity.x += JUMP_VELOCITY_X;
    }
    velocity.y = JUMP_VELOCITY_Y;
    return;
  }
  if (moves[InputAction::MOVE_RIGHT]) {
    state = RamaState::WALKING;
    position.x += timeElapsed * speed * SCALE_FACTOR;
    if (velocity.x < MAX_RUNNING_VELOCITY) {
      velocity.x++;
    }
    for (const auto &block : blocks) {
      if (onBlock && weightBearingBlock == (&block)) {
        continue;
      }
      if (doesCollide(boundingBoxFor(position), block.bounds)) {
        position.x = block.bounds.x - RAMA_WIDTH;
      }
    }
  }
  if (moves[InputAction::MOVE_LEFT]) {
    state = RamaState::WALKING;
    position.x -= timeElapsed * speed * SCALE_FACTOR;
    for (const auto &block : blocks) {
      if (onBlock && weightBearingBlock == (&block)) {
        continue;
      }
      if (doesCollide(boundingBoxFor(position), block.bounds)) {
        position.x = block.bounds.x + block.bounds.width;
      }
    }
  }

  if (!(moves[InputAction::MOVE_RIGHT] || moves[InputAction::MOVE_LEFT])) {
    state = RamaState::IDLE;
  }
}

void Rama::updateIdle(bool *moves, CollidableObjects &collidables,
                      TimeMillis &timeElapsed) {
  velocity.x = 0.0;
  BlockVect &blocks = collidables.blocks;
  if (moves[InputAction::FIRE]) {
    state = RamaState::FIRING;
    numArrowsToFire = 1;
    stateTimer = 0;
    return;
  }
  if (moves[InputAction::DOUBLE_FIRE]) {
    state = RamaState::FIRING;
    numArrowsToFire = 2;
    stateTimer = 0;
    return;
  }
  if (moves[InputAction::JUMP]) {
    ofLog(OF_LOG_NOTICE, "Initiating jump");
    state = RamaState::JUMPING;
    if (moves[InputAction::MOVE_RIGHT]) {
      velocity.x = JUMP_VELOCITY_X;
    }
    velocity.y = JUMP_VELOCITY_Y;
  }
  if (moves[InputAction::MOVE_RIGHT]) {
    state = RamaState::WALKING;
    position.x += timeElapsed * speed * SCALE_FACTOR;
  }
  if (moves[InputAction::MOVE_LEFT]) {
    state = RamaState::WALKING;
    position.x -= timeElapsed * speed * SCALE_FACTOR;
  }
}

bool Rama::update(bool *moves, CollidableObjects &collidables,
                  TimeMillis &timeElapsed) {
  switch (state) {
  case RamaState::IDLE:
    updateIdle(moves, collidables, timeElapsed);
    break;
  case RamaState::WALKING:
    updateWalking(moves, collidables, timeElapsed);
    break;
  case RamaState::JUMPING:
    updateJumping(moves, collidables, timeElapsed);
    break;
  case RamaState::FIRING:
    updateFiring(moves, collidables, timeElapsed);
    break;
  case RamaState::FIRED:
    updateFired(moves, collidables, timeElapsed);
    break;
  }

  velocityX = std::to_string(velocity.x);
  // Remove arrows that have already struck something
  std::vector<size_t> arrowsToErase;
  for (size_t i = 0; i < arrowsInFlight.size(); i++) {
    if (arrowsInFlight[i].state == ArrowState::STRUCK) {
      arrowsToErase.push_back(i);
    }
  }
  for (auto index : arrowsToErase) {
    arrowsInFlight.erase(arrowsInFlight.begin() + index);
  }

  for (auto &arrow : arrowsInFlight) {
    if (arrow.state == ArrowState::AIRBORNE) {
      arrow.update(timeElapsed);
      for (const auto &block : collidables.blocks) {
        if (doesCollide(arrow.bounds(), block.bounds)) {
          arrow.state = ArrowState::STRUCK;
        }
      }
      for (auto &rakshas : collidables.rakshases) {
        if (doesCollide(arrow.bounds(), rakshas.getBounds())) {
          rakshas.arrowStrike(arrow);
          arrow.state = ArrowState::STRUCK;
        }
      }
    }
  }

  for (auto &rakshas : collidables.rakshases) {
    if (rakshas.isAlive()) {
      if (doesCollide(boundingBoxFor(position), rakshas.getBounds())) {
        health--;
      }
    }
  }
  healthIndicator = health;
  return (health > 0);
}

void Rama::drawRama(ofImage &image, ofRectangle &bounds, ofPoint &bottomLeft) {
  image.draw(position.x - bottomLeft.x,
             bounds.height - RAMA_HEIGHT - position.y + bottomLeft.y +
                 IMAGE_PIXEL_ADJUSTMENT,
             RAMA_WIDTH, RAMA_HEIGHT);
}

void Rama::drawFiring(const long long &timeElapsed, ofRectangle &bounds,
                      ofPoint &bottomLeft) {
  drawRama(ramaFiring, bounds, bottomLeft);
}

void Rama::drawFired(const long long &timeElapsed, ofRectangle &bounds,
                     ofPoint &bottomLeft) {
  drawRama(ramaFired, bounds, bottomLeft);
}

void Rama::drawIdle(const long long &timeElapsed, ofRectangle &bounds,
                    ofPoint &bottomLeft) {
  drawRama(ramaIdle, bounds, bottomLeft);
}

void Rama::drawJumping(const long long &timeElapsed, ofRectangle &bounds,
                       ofPoint &bottomLeft) {
  drawRama(ramaJumping, bounds, bottomLeft);
}

void Rama::drawWalking(const long long &timeElapsed, ofRectangle &bounds,
                       ofPoint &bottomLeft) {
  constexpr int cycleTime = 1000;
  constexpr float periodLength = (1.0 / 6) * cycleTime;
  const int timeElapsedPeriod = timeElapsed % cycleTime;

  if (timeElapsedPeriod > (5 * periodLength)) {
    drawRama(ramaWalk1, bounds, bottomLeft);
  } else if (timeElapsedPeriod > (4 * periodLength)) {
    drawRama(ramaWalk2, bounds, bottomLeft);
  } else if (timeElapsedPeriod > (3 * periodLength)) {
    drawRama(ramaWalk3, bounds, bottomLeft);
  } else if (timeElapsedPeriod > (2 * periodLength)) {
    drawRama(ramaWalk4, bounds, bottomLeft);
  } else if (timeElapsedPeriod > (1 * periodLength)) {
    drawRama(ramaWalk5, bounds, bottomLeft);
  } else {
    drawRama(ramaWalk6, bounds, bottomLeft);
  }
}

void Rama::draw(const long long &timeElapsed, ofRectangle &bounds,
                ofPoint &bottomLeft) {

  characterHud.setPosition(position.x - bottomLeft.x,
                              bounds.height - 300 - position.y + bottomLeft.y);
  //healthIndicator.draw();
  characterHud.draw();
  //characterHud.setPosition(position.x, 500 - position.y);
  //characterHud.draw();
  if (state == RamaState::IDLE) {
    drawIdle(timeElapsed, bounds, bottomLeft);
  } else if (state == RamaState::JUMPING) {
    drawJumping(timeElapsed, bounds, bottomLeft);
  } else if (state == RamaState::WALKING) {
    drawWalking(timeElapsed, bounds, bottomLeft);
  } else if (state == RamaState::FIRING) {
    drawFiring(timeElapsed, bounds, bottomLeft);
  } else if (state == RamaState::FIRED) {
    drawFired(timeElapsed, bounds, bottomLeft);
  }

  for (auto &arrow : arrowsInFlight) {
    arrow.draw(bounds, bottomLeft);
  }
}

}
