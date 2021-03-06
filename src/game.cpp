#include "game.h"
#include "constants.h"
#include "rakshas.h"

namespace Ramayana {

Block::Block(ofRectangle _bounds, ofImage &_texture)
    : bounds(_bounds), texture(_texture) {}

Game::Game(ofPoint initialPlayerPosition, ofRectangle gameBounds)
    : bottomLeft(ofPoint(0, 0, 0)), bounds(gameBounds),
      rama(initialPlayerPosition), ended(false) {

  // Later on all these should be loaded from instances of Level objects
  backgroundImage.loadImage("tatakasForest.png");
  ofImage basicBlockImage;
  basicBlockImage.loadImage("block.png");
  blockImages.push_back(basicBlockImage);

  blocks.push_back(Block(ofRectangle(250, 100, 100, 50), blockImages[0]));

  blocks.push_back(Block(ofRectangle(650, 50, 100, 50), blockImages[0]));
  blocks.push_back(Block(ofRectangle(650, 100, 100, 50), blockImages[0]));

  blocks.push_back(Block(ofRectangle(1200, 25, 100, 50), blockImages[0]));
  blocks.push_back(Block(ofRectangle(1200, 75, 100, 50), blockImages[0]));
  blocks.push_back(Block(ofRectangle(1200, 125, 100, 50), blockImages[0]));


  blocks.push_back(Block(ofRectangle(2000, 150, 100, 50), blockImages[0]));

  blocks.push_back(Block(ofRectangle(2200, 175, 400, 250), blockImages[0]));
  blocks.push_back(Block(ofRectangle(2600, 175, 800, 50), blockImages[0]));

  rakshases.push_back(Rakshas(ofPoint(1800, 20), ofPoint(1400, 20), 5000L));
  rakshases.push_back(Rakshas(ofPoint(1000, 20), ofPoint(1000, 420), 10000L));

  rakshases.push_back(Rakshas(ofPoint(2500, 20), ofPoint(3500, 20), 3000L));
  rakshases.push_back(Rakshas(ofPoint(2600, 20), ofPoint(3500, 20), 2000L));
  rakshases.push_back(Rakshas(ofPoint(2700, 20), ofPoint(3500, 20), 1000L));
}

Game::~Game() {}

void Game::removeDeadRakshases(){
  std::vector<size_t> indicesToRemove;
  for (size_t i=0; i<rakshases.size(); i++){
    if (!rakshases[i].isAlive()) {
      indicesToRemove.push_back(i);
    }
  }

  for(auto index : indicesToRemove) {
    rakshases.erase(rakshases.begin() + index);
  }
}

void Game::update(std::vector<Ramayana::InputAction> &movesForFrame,
                  const long long &timeElapsed) {

  CollidableObjects collidables(blocks, rakshases);
  removeDeadRakshases();

  for (int i = 0; i < InputAction::NUM_ACTIONS; i++) {
    actionsEnabled[i] = false;
  }

  for (const auto inputAction : movesForFrame) {
    actionsEnabled[inputAction] = true;
  }

  for (auto &rakshas : rakshases) {
    rakshas.update(timeElapsed);
  }

  if (!rama.update(actionsEnabled, collidables, timeElapsed)) {
    ended = true;
  }

  if ((rama.position.x - bottomLeft.x) > (0.75 * bounds.width)) {
    bottomLeft.x += (bounds.width * 0.5);
  }
  if ((rama.position.y - bottomLeft.y) > 400) {
    bottomLeft.y += 300;
  }
  if (rama.position.y < bottomLeft.y) {
    bottomLeft.y -= 300;
  }

}

void Game::draw(const long long &timeElapsed) {
  const double scaleFactor =
      (double) bounds.height / (double) backgroundImage.getHeight();
  //ofLogNotice() << "Scale factor = " << scaleFactor;
  backgroundImage.draw(0, 0, backgroundImage.getWidth() * scaleFactor,
                       bounds.height);
  rama.draw(timeElapsed, bounds, bottomLeft);

  for (auto &rakshas : rakshases) {
    rakshas.draw(bounds, bottomLeft);
  }

  for (const auto &block : blocks) {
    block.texture.draw(block.bounds.x - bottomLeft.x,
                       bounds.height - block.bounds.y - block.bounds.height + bottomLeft.y,
                       block.bounds.width, block.bounds.height);
  }

}

}
