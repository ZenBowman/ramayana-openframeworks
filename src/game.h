#ifndef RAMAYANA_GAME_H
#define RAMAYANA_GAME_CPP

#include "ofMain.h"

namespace Ramayana
{

    class Game
    {
    public:
        void update();
        void draw(const long long &timeElapsed);
        Game(ofPoint initialPlayerPosition, ofRectangle gameBounds);
        ~Game();

    private:
        ofPoint playerPosition;
        ofRectangle bounds;

        ofImage ramaIdle;
        ofImage ramaWalk1;
        ofImage ramaWalk2;

    };

}

#endif
