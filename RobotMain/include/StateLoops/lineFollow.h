#pragma once

#include "Adafruit_SSD1306.h"
#include "StateLoops/State.h"

namespace StateLoops{
    class LineFollow : public State {
        public:
            //LineFollow(Hardware::DriveTrain)
            void loop();
        private:
            enum lineFollowStates{online,offline} lineFollowState;
            void findIR();
            void findPost();
            void findGauntlet();
            void findLine();
    };
}
