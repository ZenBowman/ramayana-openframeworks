#include "game.h"
#include "constants.h"

namespace Ramayana {

Block::Block(ofRectangle _bounds, ofImage &_texture)
    : bounds(_bounds), texture(_texture) {}

Game::Game(ofPoint initialPlayerPosition, ofRectangle gameBounds)
    : bottomLeft(ofPoint(0, 0, 0)), bounds(gameBounds),
      rama(initialPlayerPosition) {

  // Later on all these should be loaded from instances of Level objects
  backgroundImage.loadImage("ayodhya.png");
  ofImage basicBlockImage;
  basicBlockImage.loadImage("block.png");
  blockImages.push_back(basicBlockImage);

  blocks.push_back(Block(ofRectangle(300, 100, 100, 50), blockImages[0]));
}

Game::~Game() {}

void Game::update(std::vector<Ramayana::InputAction> &movesForFrame,
                  const long long &timeElapsed) {
  rama.update(movesForFrame, blocks, timeElapsed);
  if ((rama.position.x - bottomLeft.x) > (0.75 * bounds.width)) {
    bottomLeft.x += (bounds.width * 0.5);
  }

}

void Game::draw(const long long &timeElapsed) {
  const double scaleFactor = WINDOW_HEIGHT / backgroundImage.getHeight();
  backgroundImage.draw(0 - bottomLeft.x, 0,
                       backgroundImage.getWidth() * scaleFactor, WINDOW_HEIGHT);
  rama.draw(timeElapsed, bounds, bottomLeft);

  for (const auto &block : blocks) {
    block.texture.draw(block.bounds.x - bottomLeft.x,
                       WINDOW_HEIGHT - block.bounds.y - block.bounds.height,
                       block.bounds.width, block.bounds.height);
  }

}

}
