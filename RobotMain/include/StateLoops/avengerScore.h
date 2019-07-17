#pragma once

#include "StateLoops/State.h"
#include "Hardware/HardwareInterface.h"

namespace StateLoops {
    class AvengerScore : public State {
        public:
            void loop();
            AvengerScore();
        private:
            enum avengerScoreStates{} avengerScoreState;
            void goToBeacon();
            void dumpAvengers();
            HardwareInterface* HI;
    };
}