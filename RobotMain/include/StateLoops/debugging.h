#pragma once

#include "StateLoops/State.h"
#include "Hardware/HardwareInterface.h"
#include <Wire.h>
#include <queue>

namespace StateLoops {
    class Debugging : public State {
        public:
            Debugging();
            void loop();
        private:
            HardwareInterface* HI;
    };
}