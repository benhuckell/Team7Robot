#pragma once

#include "StateLoops/State.h"
#include "Hardware/HardwareInterface.h"

namespace StateLoops {
    class StoneCollect : public State {
        public:
            void loop();
            StoneCollect();
        private:
            enum stoneCollectStates{} stoneCollectState;
            enum postNumbers{postOne, postTwo, postThree} postNumber;
            void goToPost(enum postNumbers);
            void raiseIntake(enum postNumbers);
            void intakeStone();
            void returnToTape();
            HardwareInterface* HI;
    };
}