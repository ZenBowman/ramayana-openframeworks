#include "game.h"

namespace Ramayana
{



    Game::Game(ofPoint initialPlayerPosition, ofRectangle gameBounds)
        : playerPosition(initialPlayerPosition), bounds(gameBounds)
    {
        ramaIdle.loadImage("ramaIdle.png");
        ramaWalk1.loadImage("ramaWalk1.png");
        ramaWalk2.loadImage("ramaWalk2.png");
    }

    Game::~Game()
    {
    }

    void Game::update()
    {
    }

    void Game::draw(const long long &timeElapsed)
    {
        const int timeElapsedPeriod = ofGetElapsedTimeMillis() % 1000;

        ofLog(OF_LOG_NOTICE, "Time elapsed = %f", timeElapsed);

        if (timeElapsedPeriod > 666) {
            ramaIdle.draw(playerPosition.x, bounds.y + bounds.height - 200 - playerPosition.y, 100, 200);
        }
        else if (timeElapsedPeriod > 333) {
            ramaWalk1.draw(playerPosition.x, bounds.y + bounds.height - 200 - playerPosition.y, 100, 200);
        }
        else {
            ramaWalk2.draw(playerPosition.x, bounds.y + bounds.height - 200 - playerPosition.y, 100, 200);
        }

    }


}
