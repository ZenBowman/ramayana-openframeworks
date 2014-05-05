#ifndef RAMAYANA_GAME_H
#define RAMAYANA_GAME_CPP

#include "ofMain.h"
#include "patternrecognizer.h"
#include "rama.h"

namespace Ramayana {

struct Block {
  Block(ofRectangle _bounds, ofImage &_texture);
  ofRectangle bounds;
  ofImage &texture;
};

class Game {
public:
  void update(std::vector<Ramayana::InputAction> &movesForFrame,
              const long long &timeElapsed);
  void draw(const long long &timeElapsed);
  Game(ofPoint initialPlayerPosition, ofRectangle gameBounds);
  ~Game();

private:
  ofPoint bottomLeft;
  ofRectangle bounds;
  Rama rama;
  ofImage backgroundImage;

  std::vector<Block> blocks;
  std::vector<ofImage> blockImages;

  bool actionsEnabled[InputAction::NUM_ACTIONS];
};

}

#endif
