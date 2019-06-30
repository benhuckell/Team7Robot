#pragma once

#include "StateLoops/State.h"

namespace StateLoops {
    class StoneCollect : public State {
        public:
            void loop();
        private:
            enum stoneCollectStates{} stoneCollectState;
            enum postNumbers{postOne, postTwo, postThree} postNumber;
            void goToPost(enum postNumbers);
            void raiseIntake(enum postNumbers);
            void intakeStone();
            void returnToTape();
    };
}