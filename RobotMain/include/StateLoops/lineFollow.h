#pragma once

#include <Wire.h>
#include <queue>
#include <vector>
#include "StateLoops/State.h"
#include "Hardware/HardwareInterface.h"

namespace StateLoops{
    class LineFollow : public State {
        public:
            LineFollow();
            void loop();
            void setup();
            enum Position {LeftStart, LeftGauntlet, LeftIntersection, Post1, Post2, Post3, Post4, Post5, Post6, RightIntersection, RightGauntlet, RightStart};
            Position startingPosition;

        private:
            float getLinePositionError(bool followRightEdge);
            float getWeightedError();
            void findIR();
            void followTape(int robotSpeed, bool followRightEdge);
            void setMotorSpeeds();
            bool detectLine();
            bool detectJunction();
            void turnOnLine();
            void stopMoving();

            //varying data
            float error;
            int LSpeed;
            int RSpeed;
            int robotSpeed = 35;
            float I_sum = 0; //cummulative sum
            std::queue<float> errorHistory; //holds history of recorded line errors
            HardwareInterface* HI;

            //constant data
            float P_gain = 0; // K_p
            float I_gain = 0; // K_i
            float D_gain = 0; // K_d
            static const int numSensors = 8;
            float positionVector[numSensors] = { -30.5 ,-18.0 ,-8.4, -1.75, 1.75, 8.4, 18.0, 30.5 };
            const float maxISum = 2; //max sum to avoid integral windup
            const unsigned int ERROR_HISTORY_SIZE = 2; //max size of error queue
            const float straightLineCorrectionFactor = 1.4;
            const float ticksPerAngle = 0.25;//HI->REncoder->ticksPerRotation/wheelCircumference/(wheelCircumference/360); // ticks/rot * rot/m * m/deg 
            const float wheelCircumference = PI*0.055; //metres

            //NAVIGATION
            
            Position PostPriority[6] = {Post1, Post2, Post3, Post5, Post6, Post4};
            
            std::queue<Position> destinationList;
            Position destination;// = PostPriority[0];
            Position prevPosition;
            Position currentPosition;
            Position nextPos;
            int nextAngle;
            bool goingToGauntlet;
            bool returningToCentre;
            bool stoneCollected = false;
            bool postDetected;

            // LEFT AND RIGHT WHEN LOOKING IN SAME DIRECTION AS ROBOT IN START POSITION 
            // GUIDE //
            //enum Position {LeftStart, LeftGauntlet, LeftIntersection, Post1, Post2, Post3, Post4, Post5, Post6, RightIntersection, RightGauntlet, RightStart};
            //enum dir {CCW, CW};
            
            //!!WARNING!! DO NOT USE WHEN nextPos == destination && destination is a post. In this case the turn angle will be found automatically. 
            //  ^^This case is not covered by this array and may return an incorrect value
            //nextTurnAngle[currentPosition][dir][nextPos == destination]   
            int nextTurnAngle[12][2][2] = 
                {{{-25,0},{-25,0}},             //LeftStart
                {{15,15},{-20,-20}},            //LeftGauntlet
                {{-10,-110},{10,-110}},         //LeftIntersection
                {{-110,20},{-10,-110}},          //Post1
                {{-10,90},{-10,-90}},           //Post2
                {{-10,90},{10,-90}},            //Post3
                {{10,90},{110,-30}},            //Post4
                {{-10,90},{-10,120}},           //Post5
                {{-120,5},{-10,-90}},           //Post6
                {{-10,120},{10,120}},           //RightIntersection
                {{45,45},{-20,-20}},            //RightGauntlet
                {{60,-5},{60,-5}}};             //RightStart*/

            //nextPosition[currentPosition][dir][currentPosition == destination]
            Position nextPosition[12][2][2] = 
                {{{LeftGauntlet,LeftGauntlet},{LeftGauntlet,LeftGauntlet}},            //LeftStart
                {{LeftIntersection,LeftGauntlet},{LeftIntersection,LeftGauntlet}},     //LeftGauntlet
                {{Post5,LeftGauntlet},{Post1,LeftGauntlet}},                           //LeftIntersection
                {{LeftIntersection,Post1},{Post2,Post1}},                              //Post1
                {{Post1,Post2},{Post3,Post2}},                                         //Post2
                {{Post2,Post3},{Post4,Post3}},                                         //Post3
                {{Post3,Post4},{RightIntersection,Post4}},                             //Post4
                {{Post6,Post5},{LeftIntersection,Post5}},                              //Post5
                {{RightIntersection,Post6},{Post5,Post6}},                             //Post6
                {{Post4,RightGauntlet},{Post6,RightGauntlet}},                         //RightIntersection
                {{RightIntersection,RightGauntlet},{RightIntersection,RightGauntlet}}, //RightGauntlet
                {{RightGauntlet,RightGauntlet},{RightGauntlet,RightGauntlet}}};        //RightStart*/

            //state data
            bool climbedRamp = false;
            //enum lastIntersection = {LeftGauntlet, LeftCenter, RightCenter, RightCetner};
    };
}