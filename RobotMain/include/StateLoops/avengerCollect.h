#pragma once

#include "StateLoops/State.h"
#include "Hardware/HardwareInterface.h"

using namespace StateLoops;

class AvengerCollect : public State{
    public:
        void loop();
        AvengerCollect();
    private:
        enum avengerCollectStates{} avengerCollectState;
        void intakeAvengers();
        HardwareInterface* HI;
};