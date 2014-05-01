#ifndef PATTERN_RECOGNIZER_H
#define PATTERN_RECOGNIZER_H

#include "ofxOpenCv.h"

namespace Ramayana
{

    enum InputAction
    {
        MOVE_RIGHT = 0,
        MOVE_LEFT,
        JUMP
    };

    class PatternRecognizer
    {
        std::vector<InputAction> provideActions(cv::Mat &sourceImage);
    };

}

#endif
