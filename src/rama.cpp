#include "rama.h"

#include "ofMain.h"
#include "patternrecognizer.h"
#include "matrixops.h"
#include "game.h"

namespace Ramayana {

const int RAMA_WIDTH = 100;
const int RAMA_HEIGHT = 100;

const int MAX_JUMP_HEIGHT = 200;
const int MIN_Y_POSITION = 20;
const double SCALE_FACTOR = 0.05;
const int JUMP_VELOCITY_X = 3;
const int JUMP_VELOCITY_Y = 20;

void IdleStateBehavior::draw(const long long &timeElapsed, ofRectangle &bounds,
                             ofPoint &bottomLeft, Rama *rama) {
  ramaIdle.draw(
      rama->position.x - bottomLeft.x,
      bounds.y + bounds.height - RAMA_HEIGHT - rama->position.y + bottomLeft.y,
      RAMA_WIDTH, RAMA_HEIGHT);

}

Rama::Rama(ofPoint initialPosition)
    : position(initialPosition), speed(5), velocity(ofVec2f(0, 0)) {
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

void Rama::update(std::vector<Ramayana::InputAction> &movesForFrame,
                  std::vector<Block> &blocks, const long long &timeElapsed) {
  //ofLog(OF_LOG_NOTICE, "x = %f, y = %f", position.x, position.y);

  positionX.setup("X", std::to_string(position.x));
  positionY.setup("Y", std::to_string(position.y));
  velocityX.setup("Velocity.X", std::to_string(velocity.x));
  velocityY.setup("Velocity.Y", std::to_string(velocity.y));

  bool movingRight = false;
  bool initiatedJumpInFrame = false;
  for (const auto action : movesForFrame) {
    if (action == InputAction::JUMP) {
      if (state != RamaState::JUMPING) {
        ofLog(OF_LOG_NOTICE, "Initiating jump");
        state = RamaState::JUMPING;
        if (movingRight) {
          velocity.x = JUMP_VELOCITY_X;
        }
        velocity.y = JUMP_VELOCITY_Y;
        initiatedJumpInFrame = true;
      }
    }
    if (action == InputAction::MOVE_RIGHT) {
      if (state != RamaState::JUMPING) {
        movingRight = true;
        state = RamaState::WALKING;
        position.x += timeElapsed * speed * SCALE_FACTOR;
        for (const auto &block : blocks) {
          ofLog(OF_LOG_NOTICE, "Block bounds = (%f, %f, %f, %f)",
                block.bounds.x, block.bounds.y, block.bounds.width,
                block.bounds.height);
          ofLog(OF_LOG_NOTICE, "Player bounds = (%f, %f, %d, %d)", position.x,
                position.y, RAMA_WIDTH, RAMA_HEIGHT);
          if (MatrixOperations::doesCollide(
                  ofRectangle(position.x, position.y, RAMA_WIDTH, RAMA_HEIGHT),
                  block.bounds)) {
            position.x = block.bounds.x - RAMA_WIDTH;
          }
        }
      } else if (initiatedJumpInFrame) {
        velocity.x = JUMP_VELOCITY_X;
      }
    }
  }
  if (state == RamaState::WALKING) {
    if (!movingRight) {
      state = RamaState::IDLE;
    }
  } else if (state == RamaState::JUMPING) {
    position.x += velocity.x * SCALE_FACTOR * timeElapsed;
    position.y += velocity.y * SCALE_FACTOR * timeElapsed;
    velocity.y -= SCALE_FACTOR * timeElapsed; // gravity
    if (position.y < MIN_Y_POSITION) {
      position.y = MIN_Y_POSITION;
      state = RamaState::IDLE;
      velocity.y = 0;
      velocity.x = 0;
    }
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
