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
            enum Direction {CCW, CW};
            Position startingPosition;
            enum Turn {LEdgeTurn, REdgeTurn, QRD_Left, QRD_Right, PostTurn}; 


        private:
            float getLinePositionError(bool followRightEdge);
            float getWeightedError();
            float getWeightedEdgeError(bool followRightEdge);
            void findIR();
            void findGauntlet();
            void followTape(int robotSpeed, bool followRightEdge, bool edgeFollow);
            void setMotorSpeeds();
            bool detectLine();
            bool detectJunction();
            bool detectIntersection();
            void intersectionTurn();
            void turnTowardsPost();
            void turnXDegrees(int angle);
            void turnOnLine();
            void stopMoving();
            void junctionTurn(Turn turn);
            void QRDTurn(bool turnDirection);

            //varying data
            float error;
            int LSpeed;
            int RSpeed;
            int robotSpeed;
            float I_sum = 0; //cummulative sum

            HardwareInterface* HI;

            //constant data
            float P_gain = 1.8; // K_p
            float I_gain = 0; // K_i
            float D_gain = 15.5;//14; // K_d
            float P_gain_edge = 1.4;
            float D_gain_edge = 14.5;
            static const int numSensors = 8;
            float positionVector[numSensors] = { -30.5 ,-18.0 ,-8.4, -1.75, 1.75, 8.4, 18.0, 30.5 };
            const float maxISum = 2; //max sum to avoid integral windup
            const unsigned int ERROR_HISTORY_SIZE = 2; //max size of error queue
            const float straightLineCorrectionFactor = 1.0;
            const float ticksPerAngle = 0.25;//HI->REncoder->ticksPerRotation/wheelCircumference/(wheelCircumference/360); // ticks/rot * rot/m * m/deg 
            const float wheelCircumference = PI*0.055; //metres
            const unsigned int edgeFollowTimeout = 300;

            //NAVIGATION
            //Path lists
            Turn path1[3] = {LEdgeTurn,LEdgeTurn,PostTurn};
            int turnStep = 0;
            int junctionHandling = false;

            
            Position PostPriority[6] = {Post1, Post2, Post3, Post5, Post6, Post4};
            
            std::queue<Position> destinationList;
            Position destination;// = PostPriority[0];
            Direction dir = CW;//0 is CW, 1 is CCW
            Position prevPosition;
            Position currentPosition;
            Position nextPos;
            int nextAngle;
            bool goingToGauntlet;
            bool returningToCentre;
            bool stoneCollected = false;
            bool postDetected;
            int lineLostFactor = 22;
            int lineFoundFactor = 20;
            bool lostLine = false;

            // LEFT AND RIGHT WHEN LOOKING IN SAME DIRECTION AS ROBOT IN START POSITION 
            // GUIDE //
            //enum Position {LeftStart, LeftGauntlet, LeftIntersection, Post1, Post2, Post3, Post4, Post5, Post6, RightIntersection, RightGauntlet, RightStart};
            //enum dir {CCW, CW};
            
            
            //!!WARNING!! DO NOT USE WHEN nextPos == destination && destination is a post. In this case the turn angle will be found automatically. 
            //  ^^This case is not covered by this array and may return an incorrect value
            //nextTurnAngle[currentPosition][dir][nextPos == destination]   
                 //CCW//    //CW//
            int nextTurnAngle[12][2][2] = 
                {{{-40,0},{-37,0}},             //LeftStart
                {{-15,-15},{-7,-7}},            //LeftGauntlet
                {{-10,-110},{10,-140}},         //LeftIntersection
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