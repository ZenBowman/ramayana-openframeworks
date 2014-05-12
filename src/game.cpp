#include "game.h"
#include "constants.h"
#include "rakshas.h"

namespace Ramayana {

Block::Block(ofRectangle _bounds, ofImage &_texture)
    : bounds(_bounds), texture(_texture) {}

Game::Game(ofPoint initialPlayerPosition, ofRectangle gameBounds)
    : bottomLeft(ofPoint(0, 0, 0)),
      bounds(gameBounds),
      rama(initialPlayerPosition) {

  // Later on all these should be loaded from instances of Level objects
  backgroundImage.loadImage("tatakasForest.png");
  ofImage basicBlockImage;
  basicBlockImage.loadImage("block.png");
  blockImages.push_back(basicBlockImage);

  blocks.push_back(Block(ofRectangle(300, 100, 100, 50), blockImages[0]));

  blocks.push_back(Block(ofRectangle(550, 50, 100, 50), blockImages[0]));
  blocks.push_back(Block(ofRectangle(550, 100, 100, 50), blockImages[0]));

  blocks.push_back(Block(ofRectangle(1200, 25, 100, 50), blockImages[0]));
  blocks.push_back(Block(ofRectangle(1200, 75, 100, 50), blockImages[0]));
  blocks.push_back(Block(ofRectangle(1200, 125, 100, 50), blockImages[0]));

  rakshases.push_back(Rakshas(ofPoint(2000, 20), ofPoint(1400, 20), 10000L));

}

Game::~Game() {}

void Game::update(std::vector<Ramayana::InputAction> &movesForFrame,
                  const long long &timeElapsed) {

CollidableObjects collidables(blocks, rakshases);
  for (int i = 0; i < InputAction::NUM_ACTIONS; i++) {
    actionsEnabled[i] = false;
  }

  for (const auto inputAction : movesForFrame) {
    actionsEnabled[inputAction] = true;
  }

  for (auto &rakshas: rakshases) {
    rakshas.update(timeElapsed);
  }

  rama.update(actionsEnabled, collidables, timeElapsed);
  if ((rama.position.x - bottomLeft.x) > (0.75 * bounds.width)) {
    bottomLeft.x += (bounds.width * 0.5);
  }

}

void Game::draw(const long long &timeElapsed) {
  const double scaleFactor =
      (double) bounds.height / (double) backgroundImage.getHeight();
  //ofLogNotice() << "Scale factor = " << scaleFactor;
  backgroundImage.draw(0, 0, backgroundImage.getWidth() * scaleFactor,
                       bounds.height);
  rama.draw(timeElapsed, bounds, bottomLeft);

  for (auto &rakshas: rakshases) {
    rakshas.draw(bounds, bottomLeft);
  }

  for (const auto &block : blocks) {
    block.texture.draw(block.bounds.x - bottomLeft.x,
                       bounds.height - block.bounds.y - block.bounds.height,
                       block.bounds.width, block.bounds.height);
  }

}

}
