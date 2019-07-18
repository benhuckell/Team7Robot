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
            void findGauntlet();
            void followTape(int robotSpeed, bool followRightEdge);
            void setMotorSpeeds();
            void turn180Degrees();
            bool detectLine();
            bool detectPost();
            bool detectIntersection();
            void intersectionTurn(bool dir);

            //varying data
            int LSpeed;
            int RSpeed;
            bool postDetected = false;
            bool lostLine = false;
            float I_sum = 0; //cummulative sum
            std::queue<float> errorHistory; //holds history of recorded line errors
            HardwareInterface* HI;

            //constant data
            int P_gain = 0; // K_p
            int I_gain = 0; // K_i
            int D_gain = 0; // K_d
            static const int numSensors = 4;
            const float maxISum = 2; //max sum to avoid integral windup
            const unsigned int ERROR_HISTORY_SIZE = 2; //max size of error queue
            const float POST_TAPE_WIDTH = 4; //sensors per post tape width
            const float straightLineCorrectionFactor = 1.05;

            //state data
            bool climbedRamp = false;
    };
}
