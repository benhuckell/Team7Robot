#pragma once

#include <Wire.h>
#include <queue>
#include "StateLoops/State.h"
#include "Hardware/HardwareInterface.h"

namespace StateLoops{
    class LineFollow : public State {
        public:
            LineFollow();
            void loop();
            
        private:
            enum lineFollowStates{online,offline} lineFollowState;
            float getLinePositionError(bool followRightEdge);
            float getWeightedError();
            void findIR();
            void findPost();
            void findGauntlet();
            void findLine();
            void followTape(int robotSpeed, bool followRightEdge);
            void setMotorSpeeds();
            float getWeightedError();

            int LSpeed;
            int RSpeed;
            bool postDetected = false;
            bool lostLine = false;
            float I_sum = 0; //cummulative sum
            std::queue<float> errorHistory; //holds history of recorded line errors
            HardwareInterface* HI;

            int P_gain = 0; // K_p
            int I_gain = 0; // K_i
            int D_gain = 0; // K_d
            static const int numSensors = 4;
            const float maxISum = 2; //max sum to avoid integral windup
            const unsigned int ERROR_HISTORY_SIZE = 2; //max size of error queue
            const float POST_TAPE_WIDTH = 4; //sensors per post tape width
            const float straightLineCorrectionFactor = 1.05;
    };
}
