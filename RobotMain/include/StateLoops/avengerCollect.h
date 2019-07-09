#pragma once

#include "StateLoops/State.h"

using namespace StateLoops;

class AvengerCollect : public State{
    public:
        void loop();
    private:
        enum avengerCollectStates{} avengerCollectState;
        void intakeAvengers();
};