#include "rama.h"

#include "ofMain.h"
#include "patternrecognizer.h"

namespace Ramayana
{

Rama::Rama(ofPoint initialPosition)
{
    position = initialPosition;
    ramaIdle.loadImage("ramaIdle.png");
    ramaWalk1.loadImage("ramaWalk1.png");
    ramaWalk2.loadImage("ramaWalk2.png");
}

Rama::~Rama()
{

}

void Rama::update(std::vector<Ramayana::InputAction> &movesForFrame, const long long &timeElapsed)
{
    bool anyMovement = false;
    for (const auto action : movesForFrame)
    {
        if (action == MOVE_RIGHT)
        {
            anyMovement = true;
            state = WALKING;
            position.x ++;

        }
    }
    if (!anyMovement) {
        state = IDLE;
    }

}

void Rama::draw(const long long &timeElapsed, ofRectangle &bounds)
{
    if (state == IDLE) {
        ramaIdle.draw(position.x, bounds.y + bounds.height - 200 - position.y, 100, 200);
    } else if (state == WALKING) {
        const int timeElapsedPeriod = timeElapsed % 1000;

        if (timeElapsedPeriod > 666) {
            ramaIdle.draw(position.x, bounds.y + bounds.height - 200 - position.y, 100, 200);
        }
        else if (timeElapsedPeriod > 333) {
            ramaWalk1.draw(position.x, bounds.y + bounds.height - 200 - position.y, 100, 200);
        }
        else {
            ramaWalk2.draw(position.x, bounds.y + bounds.height - 200 - position.y, 100, 200);
        }
    }
}

}
