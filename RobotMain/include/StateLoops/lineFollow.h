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
            bool detectJunction();
            void turnXDegrees(int angle);
            void stopMoving();

            int LSpeed;
            int RSpeed;
            bool postDetected = false;
            bool isLineLost = false; //keep: for edge case when line is lost
            float I_sum = 0; //cummulative sum
            std::queue<float> errorHistory; //holds history of recorded line errors
            HardwareInterface* HI;
            float error;

            float P_gain = 0; // K_p
            float I_gain = 0; // K_i
            float D_gain = 0; // K_d

            static const int numSensors = 8;
            const float maxISum = 2; //max sum to avoid integral windup
            const unsigned int ERROR_HISTORY_SIZE = 2; //max size of error queue
            const float POST_TAPE_WIDTH = 4; //sensors per post tape width
            const float straightLineCorrectionFactor = 1.42;
            const float turningCorrectionFactor = 2.0;
            const float ticksPerAngle = 1.75; //HI->REncoder->ticksPerRotation/wheelCircumference/(wheelCircumference/360); // ticks/rot * rot/m * m/deg
    };
}
