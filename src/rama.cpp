#include "rama.h"

#include "ofMain.h"
#include "patternrecognizer.h"
#include "matrixops.h"
#include "game.h"

using namespace MatrixOperations;

namespace Ramayana {

const int RAMA_WIDTH = 100;
const int RAMA_HEIGHT = 150;

const int MAX_JUMP_HEIGHT = 200;
const int MIN_Y_POSITION = 20;
const double SCALE_FACTOR = 0.05;
const int JUMP_VELOCITY_X = 3;
const int JUMP_VELOCITY_Y = 20;
constexpr int IMAGE_PIXEL_ADJUSTMENT = 5;
constexpr int MAX_RUNNING_VELOCITY = 5;

Rama::Rama(ofPoint initialPosition)
    : position(initialPosition),
      speed(5),
      onBlock(false),
      weightBearingBlock(nullptr),
      velocity(ofVec2f(0, 0)),
      health(100) {
  ramaIdle.loadImage("ramaIdle.png");
  ramaWalk1.loadImage("ramaWalking1.png");
  ramaWalk2.loadImage("ramaWalking2.png");
  ramaWalk3.loadImage("ramaWalking3.png");
  ramaWalk4.loadImage("ramaWalking4.png");
  ramaWalk5.loadImage("ramaWalking5.png");
  ramaWalk6.loadImage("ramaWalking6.png");
  ramaJumping.loadImage("ramaJumping.png");

  gui.setup();
  gui.add(positionX.setup("X", ""));
  gui.add(positionY.setup("Y", ""));
  gui.add(velocityX.setup("Velocity.X", ""));
  gui.add(velocityY.setup("Velocity.Y", ""));
  gui.setPosition(initialPosition);

  characterHud.setup();
  characterHud.setPosition(position);
  characterHud.add(healthIndicator.setup("Health", health, 0, maxHealth));
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

void Rama::updateJumping(bool *moves, CollidableObjects &collidables,
                         TimeMillis &timeElapsed) {
  BlockVect &blocks = collidables.blocks;
  if (state == RamaState::JUMPING) {
    position.x += velocity.x * SCALE_FACTOR * timeElapsed;
    position.y += velocity.y * SCALE_FACTOR * timeElapsed;
    velocity.y -= SCALE_FACTOR * timeElapsed;  // gravity
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
        } else {
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
    if (velocity.x < MAX_RUNNING_VELOCITY) {velocity.x++;}
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

void Rama::updateIdle(bool *moves, CollidableObjects &collidables, TimeMillis &timeElapsed) {
BlockVect &blocks = collidables.blocks;
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

void Rama::update(bool *moves, CollidableObjects &collidables, TimeMillis &timeElapsed) {
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
  }
  for (auto &rakshas: collidables.rakshases) {
    if (doesCollide(boundingBoxFor(position), rakshas.getBounds())) {
      health --;
    }
  }
  healthIndicator.setup("Health", health, 0, maxHealth);
}

void Rama::drawRama(ofImage &image, ofRectangle &bounds, ofPoint &bottomLeft) {
  image.draw(position.x - bottomLeft.x,
             bounds.height - RAMA_HEIGHT - position.y - bottomLeft.y +
                 IMAGE_PIXEL_ADJUSTMENT,
             RAMA_WIDTH, RAMA_HEIGHT);
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

  healthIndicator.setPosition(position.x - bottomLeft.x, bounds.height - 200 - position.y);
  healthIndicator.draw();
  //characterHud.setPosition(position.x, 500 - position.y);
  //characterHud.draw();
  if (state == RamaState::IDLE) {
    drawIdle(timeElapsed, bounds, bottomLeft);
  } else if (state == RamaState::JUMPING) {
    drawJumping(timeElapsed, bounds, bottomLeft);
  } else if (state == RamaState::WALKING) {
    drawWalking(timeElapsed, bounds, bottomLeft);
  }
}

}
