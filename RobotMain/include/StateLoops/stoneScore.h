#pragma once

#include "StateLoops/State.h"
#include "Hardware/HardwareInterface.h"

namespace StateLoops {
    class StoneScore : public State {
        public:
            StoneScore();
            void loop();
            enum HoleNumbers {holeOne,holeTwo,holeThree,holeFour} ;
        private:
            void goToHole(enum HoleNumbers);
            void placeStone();
            void returnToTape();
            
            HardwareInterface* HI;

            int holeTurnAngles[4] = {-20, -10, 10, 20};
    };
}