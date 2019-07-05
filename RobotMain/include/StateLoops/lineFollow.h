#pragma once

#include "Adafruit_SSD1306.h"
#include "StateLoops/State.h"

namespace StateLoops{
    class LineFollow : public State {
        public:
            LineFollow(QRD QRD_Array[QRD::NUM_QRD_SENSORS]);
            void loop();
            
        private:
            enum lineFollowStates{online,offline} lineFollowState;
            float getLinePositionError()
            void findIR();
            void findPost();
            void findGauntlet();
            void findLine();
            void followTape();

            int LSpeed;
            int RSpeed;
            int speedAdj;
            bool followRightEdge = true; //default follow right edge
            bool postDetected = false;
            bool lostLine = false;
            float I_sum = 0; //cummulative sum
            std::queue<float> errorHistory; //holds history of recorded line errors
            
            const int P_gain = 100; // K_p
            const int I_gain = 0; // K_i
            const int D_gain = 0; // K_d
            const float maxISum = 2; //max sum to avoid integral windup
            const int ERROR_HISTORY_SIZE = 10; //max size of error queue
            const float POST_TAPE_WIDTH = 4; //sensors per post tape width
    };
}
