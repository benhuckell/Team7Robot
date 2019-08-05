#pragma once

#include "StateLoops/State.h"
#include "Hardware/HardwareInterface.h"

namespace StateLoops {
    class StoneCollect : public State {
        public:
            enum postNumber{post1, post2, post3, post4, post5, post6};
            StoneCollect();
            void loop();
            void getStone_const_speed(int tickTarget);

            postNumber postList[6] = {post1, post6, post3, post4, post5, post2};
            int nextPostNum;
        private:
            HardwareInterface* HI;
            enum stoneCollectStates{} stoneCollectState;
            void goToPost(enum postNumber);
            void raiseIntake(enum postNumber);
            void intakeStone();
            void returnToTape();
    };
}