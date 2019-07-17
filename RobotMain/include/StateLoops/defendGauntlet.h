#pragma once

#include "StateLoops/State.h"
#include "Hardware/HardwareInterface.h"

namespace StateLoops {
    class DefendGauntlet : public State {
        public:
            void loop();
            DefendGauntlet();
        private:
            void defend();
            HardwareInterface* HI;
    };
}