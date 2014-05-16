#ifndef RAMAYANA_GAME_H
#define RAMAYANA_GAME_CPP

#include "ofMain.h"
#include "patternrecognizer.h"
#include "rama.h"
#include "rakshas.h"

namespace Ramayana {

struct Block {
  Block(ofRectangle _bounds, ofImage &_texture);
  ofRectangle bounds;
  ofImage &texture;
};

typedef std::vector<Block> BlockVect;

struct CollidableObjects {
  BlockVect &blocks;
  RakshasVect &rakshases;
  CollidableObjects(BlockVect &_blocks, RakshasVect &_rakshases)
      : blocks(_blocks), rakshases(_rakshases) {}
};

class Game {
public:
  void removeDeadRakshases();
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

  std::vector<Rakshas> rakshases;
  std::vector<Block> blocks;
  std::vector<ofImage> blockImages;

  bool actionsEnabled[InputAction::NUM_ACTIONS];
};

}

#endif
