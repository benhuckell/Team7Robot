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
            enum Position {LeftStart, LeftGauntlet, LeftIntersection, Post1, Post2, Post3, Post4, Post5, Post6, RightIntersection, RightGauntlet, RightStart};
            enum Direction {CCW, CW};
            Position startingPosition;

        private:
            float getLinePositionError(bool followRightEdge);
            float getWeightedError();
            void findIR();
            void findGauntlet();
            void followTape(int robotSpeed, bool followRightEdge);
            void setMotorSpeeds();
            bool detectLine();
            bool detectJunction();
            bool detectIntersection();
            void intersectionTurn();
            void turnTowardsPost();
            void turnXDegrees(int angle);
            void turnOnLine();
            void stopMoving();

            //varying data
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
            const float straightLineCorrectionFactor = 1.00;
            const float ticksPerAngle = HI->REncoder->ticksPerRotation/wheelCircumference/(wheelCircumference/360); // ticks/rot * rot/m * m/deg 
            const float wheelCircumference = PI*0.055; //metres

            //NAVIGATION
            Position PostPriority[6] = {Post1, Post2, Post3, Post5, Post6, Post4};
            std::queue<Position> CurrentPath;
            Position destination = PostPriority[0];
            Direction dir = CW;//0 is CW, 1 is CCW
            Position prevPosition;
            Position currentPosition = startingPosition;
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
            
            //int nextCenterAngle[currentPos][centerDest][dir]
            /*int turnAngleCenter[8][8][2]
                {{-110},{NULL}, {-90},{5},{5},{5}, {90},{-10}, {-5},{-5}},           //LI
                {20,NULL, NULL,-90,5,5, -110,-110, 5,NULL},                          //P1
                {-5,-5, 90,NULL,-90,-5, -5,-5, -5,-5},                               //P2
                {-5,-5, -5,90,NULL,-90, NULL,NULL, 5,5},                             //P3
                {5,5, -5,-5,90,NULL, 110,110, NULL,-30},                             //P4
                {-10,NULL, 120,120,NULL,NULL, NULL,90, -5,-5},                       //P5 
                {-5,-5, -5,NULL,NULL,-120, -90,NULL, NULL,10},                       //P6
                {5,5, -5,-5,-5,90, 5,-90, NULL,120},                                 //RI*/

            //nextTurnAngle[prevPosition][destination][dir];     
            /*int nextTurnAngleOld[12][10] = 
                {{{5},{-70}, {NULL},{NULL},{NULL},{NULL}, {NULL},{NULL}, {NULL},{NULL}}, //LeftStart
                {{NULL},{NULL}, {-30},{-30},{-30},{-30}, {15},{15}, {NULL},{NULL}},      //LeftGauntlet
                {-110,NULL, -90,5,5,5, 90,-10, -5,-5},               //LeftIntersection
                {20,NULL, NULL,-90,5,5, -110,-110, 5,NULL},          //Post1
                {-5,-5, 90,NULL,-90,-5, -5,-5, -5,-5},               //Post2
                {-5,-5, -5,90,NULL,-90, NULL,NULL, 5,5},             //Post3
                {5,5, -5,-5,90,NULL, 110,110, NULL,-30},             //Post4
                {-10,NULL, 120,120,NULL,NULL, NULL,90, -5,-5},       //Post5 
                {-5,-5, -5,NULL,NULL,-120, -90,NULL, NULL,10},       //Post6
                {5,5, -5,-5,-5,90, 5,-90, NULL,120},                 //RightIntersection
                {NULL,NULL, 45,45,45,45, -20,-20, NULL,NULL},        //RightGauntlet
                {NULL,NULL, NULL,NULL,NULL,NULL, NULL,NULL, 60,-5}}; //RightStart*/

            //!!WARNING!! DO NOT USE WHEN nextPos == destination && destination is a post. In this case the turn angle will be found automatically. 
            //  ^^This case is not covered by this array and may return an incorrect value
            //nextTurnAngle[currentPosition][dir][nextPos == destination]   
            int nextTurnAngle[12][2][2] = 
                {{{-70,5},{-70,5}},             //LeftStart
                {{15,15},{-30,-30}},            //LeftGauntlet
                {{-10,-110},{10,-110}},         //LeftIntersection
                {{-110,20},{-10,-90}},          //Post1
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
