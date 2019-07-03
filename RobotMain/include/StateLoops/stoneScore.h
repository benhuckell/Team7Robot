#pragma once

#include "StateLoops/State.h"

namespace StateLoops {
    class StoneScore : public State {
        public:
            void loop();
        private:
            enum stoneScoreStates{} stoneScoreState;
            enum holeNumbers{holeOne,holeTwo,holeThree,holeFour,holeFive} holeNumber;
            void goToGauntlet();
            void findHole(enum holeNumbers);
            void placeStone();
            void returnToTape();
    };
}