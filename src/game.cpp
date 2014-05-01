#include "game.h"

namespace Ramayana
{



    Game::Game(ofPoint initialPlayerPosition, ofRectangle gameBounds)
        : rama(initialPlayerPosition), bounds(gameBounds)
    {

    }

    Game::~Game()
    {
    }

    void Game::update(std::vector<Ramayana::InputAction> &movesForFrame, const long long &timeElapsed)
    {
        rama.update(movesForFrame, timeElapsed);

    }

    void Game::draw(const long long &timeElapsed)
    {
        rama.draw(timeElapsed, bounds);

    }


}
