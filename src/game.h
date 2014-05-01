#ifndef RAMAYANA_GAME_H
#define RAMAYANA_GAME_CPP

#include "ofMain.h"
#include "patternrecognizer.h"
#include "rama.h"

namespace Ramayana
{

    class Game
    {
    public:
        void update(std::vector<Ramayana::InputAction> &movesForFrame, const long long &timeElapsed);
        void draw(const long long &timeElapsed);
        Game(ofPoint initialPlayerPosition, ofRectangle gameBounds);
        ~Game();

    private:
        ofRectangle bounds;
        Rama rama;
    };

}

#endif
