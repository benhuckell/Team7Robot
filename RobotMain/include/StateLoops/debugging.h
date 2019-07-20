#pragma once

#include "StateLoops/State.h"
#include "Hardware/HardwareInterface.h"

namespace StateLoops {
    class Debugging : public State {
        public:
            Debugging();
            void loop();
        private:
            int numSensors = 8;
            HardwareInterface* HI;
    };
}