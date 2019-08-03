#pragma once

#include "StateLoops/State.h"
#include "Hardware/HardwareInterface.h"

namespace StateLoops {
    class StoneCollect : public State {
        public:
            StoneCollect();
            void loop();
        private:
            HardwareInterface* HI;
            enum stoneCollectStates{} stoneCollectState;
            enum postNumbers{postOne, postTwo, postThree} postNumber;
            void goToPost(enum postNumbers);
            void raiseIntake(enum postNumbers);
            void intakeStone();
            void returnToTape();
            void getStone();
            void getStone_const_speed();
            HardwareInterface* HI;
    };
}