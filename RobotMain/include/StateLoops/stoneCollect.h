#pragma once

#include "StateLoops/State.h"
#include "Hardware/HardwareInterface.h"

namespace StateLoops {
    class StoneCollect : public State {
        public:
            enum PostNumber {Post1, Post2, Post3, Post4, Post5, Post6};
            StoneCollect();
            void loop();
            int winchTickTargets[6] = {20,10,10,20,30,30};
        private:
            void returnToTape();

            HardwareInterface* HI;
    };
}