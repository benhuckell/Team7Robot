#pragma once

#include "StateLoops/State.h"

namespace StateLoops {
    class AvengerCollect : public State {
        public:
            void loop();
        private:
            enum avengerCollectStates{} avengerCollectState;
            void intakeAvengers();
    };
}