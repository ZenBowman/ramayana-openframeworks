#include "rama.h"

#include "ofMain.h"
#include "patternrecognizer.h"
#include "matrixops.h"
#include "game.h"

using namespace MatrixOperations;

namespace Ramayana {

const int RAMA_WIDTH = 100;
const int RAMA_HEIGHT = 100;

const int MAX_JUMP_HEIGHT = 200;
const int MIN_Y_POSITION = 20;
const double SCALE_FACTOR = 0.05;
const int JUMP_VELOCITY_X = 3;
const int JUMP_VELOCITY_Y = 20;

Rama::Rama(ofPoint initialPosition)
    : position(initialPosition), speed(5), velocity(ofVec2f(0, 0)),
      onBlock(false), weightBearingBlock(nullptr) {
  ramaIdle.loadImage("ramaIdle.png");
  ramaWalk1.loadImage("ramaWalk1.png");
  ramaWalk2.loadImage("ramaWalk2.png");
  ramaWalk3.loadImage("ramaWalk3.png");
  ramaWalk4.loadImage("ramaWalk4.png");

  gui.setup();
  gui.add(positionX.setup("X", ""));
  gui.add(positionY.setup("Y", ""));
  gui.add(velocityX.setup("Velocity.X", ""));
  gui.add(velocityY.setup("Velocity.Y", ""));
  gui.setPosition(initialPosition);
}

Rama::~Rama() {}

ofRectangle boundingBoxFor(const ofPoint &position) {
  ofRectangle bb;
  bb.x = position.x;
  bb.y = position.y;
  bb.width = RAMA_WIDTH;
  bb.height = RAMA_HEIGHT;
  return bb;
}

void Rama::updateJumping(bool *moves, BlockVect &blocks,
                         TimeMillis &timeElapsed) {
  if (state == RamaState::JUMPING) {
    const ofPoint oldPosition = position;
    position.x += velocity.x * SCALE_FACTOR * timeElapsed;
    position.y += velocity.y * SCALE_FACTOR * timeElapsed;
    velocity.y -= SCALE_FACTOR * timeElapsed; // gravity
    for (auto &block : blocks) {
      if (doesCollide(boundingBoxFor(position), block.bounds)) {
        RelativeDirection direction =
            getRelativePosition(boundingBoxFor(position), block.bounds);
        if (direction.isAbove) {
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

void Rama::updateWalking(bool *moves, BlockVect &blocks,
                         TimeMillis &timeElapsed) {
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
      velocity.x = JUMP_VELOCITY_X;
    }
    velocity.y = JUMP_VELOCITY_Y;
    return;
  }
  if (moves[InputAction::MOVE_RIGHT]) {
    state = RamaState::WALKING;
    position.x += timeElapsed * speed * SCALE_FACTOR;
    for (const auto &block : blocks) {
      if (onBlock && weightBearingBlock == (&block)) {
        continue;
      }
      if (doesCollide(boundingBoxFor(position), block.bounds)) {
        position.x = block.bounds.x - RAMA_WIDTH;
      }
    }
  }

  if (!moves[InputAction::MOVE_RIGHT]) {
    state = RamaState::IDLE;
  }
}

void Rama::updateIdle(bool *moves, BlockVect &blocks, TimeMillis &timeElapsed) {

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
}

void Rama::update(bool *moves, BlockVect &blocks, TimeMillis &timeElapsed) {
  switch (state) {
  case RamaState::IDLE:
    updateIdle(moves, blocks, timeElapsed);
    break;
  case RamaState::WALKING:
    updateWalking(moves, blocks, timeElapsed);
    break;
  case RamaState::JUMPING:
    updateJumping(moves, blocks, timeElapsed);
    break;
  }
}

void Rama::draw(const long long &timeElapsed, ofRectangle &bounds,
                ofPoint &bottomLeft) {
  gui.draw();
  if (state == RamaState::IDLE) {
    ramaIdle.draw(
        position.x - bottomLeft.x,
        bounds.y + bounds.height - RAMA_HEIGHT - position.y + bottomLeft.y,
        RAMA_WIDTH, RAMA_HEIGHT);
  } else if (state == RamaState::JUMPING) {
    ramaIdle.draw(
        position.x - bottomLeft.x,
        bounds.y + bounds.height - RAMA_HEIGHT - position.y + bottomLeft.y,
        RAMA_WIDTH, RAMA_HEIGHT);
  } else if (state == RamaState::WALKING) {
    const int timeElapsedPeriod = timeElapsed % 600;

    if (timeElapsedPeriod > 450) {
      ramaWalk1.draw(
          position.x - bottomLeft.x,
          bounds.y + bounds.height - RAMA_HEIGHT - position.y + bottomLeft.y,
          RAMA_WIDTH, RAMA_HEIGHT);
    } else if (timeElapsedPeriod > 300) {
      ramaWalk2.draw(
          position.x - bottomLeft.x,
          bounds.y + bounds.height - RAMA_HEIGHT - position.y + bottomLeft.y,
          RAMA_WIDTH, RAMA_HEIGHT);
    } else if (timeElapsedPeriod > 150) {
      ramaWalk3.draw(
          position.x - bottomLeft.x,
          bounds.y + bounds.height - RAMA_HEIGHT - position.y + bottomLeft.y,
          RAMA_WIDTH, RAMA_HEIGHT);
    } else {
      ramaWalk4.draw(
          position.x - bottomLeft.x,
          bounds.y + bounds.height - RAMA_HEIGHT - position.y + bottomLeft.y,
          RAMA_WIDTH, RAMA_HEIGHT);
    }
  }
}

}
