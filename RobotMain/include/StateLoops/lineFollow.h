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
            enum Position {LeftStart, LeftGauntlet, LeftIntersection, Post1, Post2, Post3, Post4, Post5, Post6, RightIntersection, RightGauntlet, RightStart};
            enum dir {CLOCKWISE, COUNTERCLOCKWISE};
            
        private:
            
            float getLinePositionError(bool followRightEdge);
            float getWeightedError();
            void findIR();
            void findGauntlet();
            void followTape(int robotSpeed, bool followRightEdge);
            void setMotorSpeeds();
            bool detectLine();
            bool detectPost();
            bool detectIntersection();
            void intersectionTurn(bool dir);
            void turnTowardsPost();
            void turnXDegrees(int angle);
            void stopRobot();
            void turnOnLine();
            bool getNextTurnAngle(Position lastPosition, Position destination);//uses stoneCollected and clockwiseMove

            //varying data
            bool team; //true for right, false for left
            int LSpeed;
            int RSpeed;
            int robotSpeed = 35;
            bool dir;//true is right, false is left
            bool stoneCollected = false;
            bool clockwiseMove;
            float I_sum = 0; //cummulative sum
            std::queue<float> errorHistory; //holds history of recorded line errors
            HardwareInterface* HI;

            //constant data
            int P_gain = 0; // K_p
            int I_gain = 0; // K_i
            int D_gain = 0; // K_d
            static const int numSensors = 8;
            const float maxISum = 2; //max sum to avoid integral windup
            const unsigned int ERROR_HISTORY_SIZE = 2; //max size of error queue
            const float straightLineCorrectionFactor = 1.05;
            const float ticksPerAngle = HI->REncoder->ticksPerRotation/wheelCircumference/(wheelCircumference/360); // ticks/rot * rot/m * m/deg 
            const float wheelCircumference = PI*0.055; //metres
    
            // LEFT AND RIGHT WHEN LOOKING IN SAME DIRECTION AS ROBOT IN START POSITION 
            // GUIDE //
            //{{{[LeftGauntlet][CCW][false],      [LeftGauntlet][CCW][true]},      {[LeftGauntlet][CW][false],      [LeftGauntlet][CW][true]}},
            // {{[LeftIntersection][CCW][false],  [LeftIntersection][CCW][true]},  {[LeftIntersection][CW][false],  [LeftIntersection][CW][true]}},
            // {{[Post1][CCW][false],             [Post1][CCW][true]},             {[Post1][CW][false],             [Post1][CW][true]}},
            // {{[Post2][CCW][false],             [Post2][CCW][true]},             {[Post2][CW][false],             [Post2][CW][true]}},
            // {{[Post3][CCW][false],             [Post3][CCW][true]},             {[Post3][CW][false],             [Post3][CW][true]}},
            // {{[Post4][CCW][false],             [Post4][CCW][true]},             {[Post4][CW][false],             [Post4][CW][true]}},
            // {{[Post5][CCW][false],             [Post5][CCW][true]},             {[Post5][CW][false],             [Post5][CW][true]}},
            // {{[Post6][CCW][false],             [Post6][CCW][true]},             {[Post6][CW][false],             [Post6][CW][true]}},
            // {{[RightIntersection][CCW][false], [RightIntersection][CCW][true]}, {[RightIntersection][CW][false], [RightIntersection][CW][true]},
            // {{[RightGauntlet][CCW][false],     [RightGauntlet][CCW][true]},     {[RightGauntlet][CW][false],     [RightGauntlet][CW][true]}}}

            //enum Position {LeftStart, LeftGauntlet, LeftIntersection, Post1, Post2, Post3, Post4, Post5, Post6, RightIntersection, RightGauntlet, RightStart};
            //enum dir {CCW, CW};
            //bool stoneCollected
            //nextTurnAngle[lastPosition][dir][nextPosition == destination]
            /*bool nextTurnAngle[12][2][2] = {{{-45,-45},{-45,-45}}, //LeftStart
                                            {{100,15},{100,-45}}, //LeftGauntlet
                                            {{15,20},{130,-10}}, //LeftIntersection
                                            {{}},//Post1
                                            {{}},//Post2
                                            {{}},//Post3
                                            {{}},//Post4
                                            {{}},//Post5
                                            {{}},//Post6
                                            {{}},//RightIntersection
                                            {{}},//RightGauntlet
                                            {{45,45},{45,45}}};//RightStart

            //nextPosition[currentPosition][dir][reachedDestination]
            Position nextPosition[10][2][2] = {{{LeftIntersection,LeftGauntlet},{LeftIntersection,LeftGauntlet}},//LeftGauntlet
                                               {{Post5,LeftGauntlet},{Post1,LeftGauntlet}},//LeftIntersection
                                               {{LeftIntersection,Post1},{Post2,Post1}},//Post1
                                               {{Post1,Post2},{Post3,Post2}},//Post2
                                               {{Post2,Post3},{Post4,Post3}},//Post3
                                               {{Post3,Post4},{RightIntersection,Post4}},//Post4
                                               {{Post6,Post5},{LeftIntersection,Post5}},//Post5
                                               {{RightIntersection,Post6},{Post5,Post6}},//Post6
                                               {{Post4,RightGauntlet},{Post6,RightGauntlet}},//RightIntersection
                                               {{RightIntersection,RightGauntlet},{RightIntersection,RightGauntlet}}};//RightGauntlet*/

            //state data
            bool climbedRamp = false;
            //enum lastIntersection = {LeftGauntlet, LeftCenter, RightCenter, RightCetner};
    };
}
