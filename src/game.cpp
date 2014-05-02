#include "game.h"

namespace Ramayana {

Game::Game(ofPoint initialPlayerPosition, ofRectangle gameBounds)
    : rama(initialPlayerPosition), bounds(gameBounds), bottomLeft(ofPoint(0,0,0)) {}

Game::~Game() {}

void Game::update(std::vector<Ramayana::InputAction> &movesForFrame,
                  const long long &timeElapsed) {
  rama.update(movesForFrame, timeElapsed);
  if ((rama.position.x - bottomLeft.x) > (0.75 * bounds.width)) {
      bottomLeft.x += (bounds.width * 0.5);
  }

}

void Game::draw(const long long &timeElapsed) {
  rama.draw(timeElapsed, bounds, bottomLeft);

}

}
