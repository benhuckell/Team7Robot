#pragma once

#include "StateLoops/State.h"

namespace StateLoops {
    class DefendGauntlet : public State {
        public:
            void loop();
        private:
            void defend();
    };
}