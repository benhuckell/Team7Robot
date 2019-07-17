#pragma once

#include "StateLoops/State.h"

namespace StateLoops {
    class StoneScore : public State {
        public:
            static void loop();
            StoneScore();
        private:
            enum stoneScoreStates{} stoneScoreState;
            enum holeNumbers{holeOne,holeTwo,holeThree,holeFour,holeFive} holeNumber;
            static void goToGauntlet();
            static void findHole(enum holeNumbers);
            static void placeStone();
            static void returnToTape();
            HardwareInterface* HI;
    };
}