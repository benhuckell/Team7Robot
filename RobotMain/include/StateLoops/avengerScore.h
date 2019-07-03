#pragma once

#include "StateLoops/State.h"

namespace StateLoops {
    class AvengerScore : public State {
        public:
            void loop();
        private:
            enum avengerScoreStates{} avengerScoreState;
            void goToBeacon();
            void dumpAvengers();
    };
}