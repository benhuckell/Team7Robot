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
            enum Turn {LEdgeTurn, REdgeTurn, QRD_Left, QRD_Right, PostTurn}; 
            Position startingPosition;

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
            void junctionTurn(Turn turn);
            void turnToPost();
            void turnXDegrees(int angle);
            void turnOnLine();
            void stopMoving();
            void QRDTurn(bool turnRight);

            //varying data
            float error;
            int LSpeed;
            int RSpeed;
            int robotSpeed = 35;
            float I_sum = 0; //cummulative sum

            HardwareInterface* HI;

            //constant data
            const unsigned int edgeFollowTimeout = 500;
            float P_gain = 1.9; // K_p
            float I_gain = 0; // K_i
            float D_gain = 16.5; // K_d
            float P_gain_edge = 1.9;
            float D_gain_edge = 13;
            static const int numSensors = 8;
            float positionVector[numSensors] = { -30.5 ,-18.0 ,-8.4, -1.75, 1.75, 8.4, 18.0, 30.5 };
            const float maxISum = 2; //max sum to avoid integral windup
            const unsigned int ERROR_HISTORY_SIZE = 2; //max size of error queue
            const float straightLineCorrectionFactor = 1.4;
            const float ticksPerAngle = 0.25;//HI->REncoder->ticksPerRotation/wheelCircumference/(wheelCircumference/360); // ticks/rot * rot/m * m/deg 
            const float wheelCircumference = PI*0.055; //metres

            //NAVIGATION
            
            Position PostPriority[6] = {Post1, Post2, Post3, Post5, Post6, Post4};
            Turn TurnTypes[5] = {LEdgeTurn, REdgeTurn, QRD_Left, QRD_Right,PostTurn};

            std::queue<Position> destinationList;
            Position destination;// = PostPriority[0];
            Direction dir = CW;//0 is CW, 1 is CCW
            Position prevPosition;
            Position currentPosition;
            Position nextPos;
            Turn nextTurn;
            bool goingToGauntlet;
            bool returningToCentre;
            bool stoneCollected = false;
            bool postDetected;
            int lineLostFactor = 15;
            int lineFoundFactor = 20;
            bool lostLine = false;

            // LEFT AND RIGHT WHEN LOOKING IN SAME DIRECTION AS ROBOT IN START POSITION 
            // GUIDE //
            //enum Position {LeftStart, LeftGauntlet, LeftIntersection, Post1, Post2, Post3, Post4, Post5, Post6, RightIntersection, RightGauntlet, RightStart};
            //enum dir {CCW, CW};
            
            //!!WARNING!! DO NOT USE WHEN nextPos == destination && destination is a post. In this case the turn angle will be found automatically. 
            //  ^^This case is not covered by this array and may return an incorrect value
            //nextTurnAngle[currentPosition][dir][nextPos == destination]   
                        //CCW//                 //CW//
            Turn nextTurnType[12][2][2] = 
                {{{LEdgeTurn,REdgeTurn},{LEdgeTurn,REdgeTurn}},             //LeftStart
                {{REdgeTurn,REdgeTurn},{LEdgeTurn,LEdgeTurn}},            //LeftGauntlet
                {{LEdgeTurn,QRD_Left},{REdgeTurn,QRD_Left}},         //LeftIntersection
                {{QRD_Left,REdgeTurn},{REdgeTurn,PostTurn}},          //Post1
                {{LEdgeTurn,PostTurn},{REdgeTurn,PostTurn}},           //Post2
                {{LEdgeTurn,PostTurn},{REdgeTurn,PostTurn}},            //Post3
                {{LEdgeTurn,PostTurn},{QRD_Right,LEdgeTurn}},            //Post4
                {{LEdgeTurn,PostTurn},{QRD_Right,LEdgeTurn}},           //Post5
                {{QRD_Left,REdgeTurn},{REdgeTurn,PostTurn}},           //Post6
                {{LEdgeTurn,QRD_Right},{REdgeTurn,QRD_Right}},           //RightIntersection
                {{REdgeTurn,REdgeTurn},{LEdgeTurn,LEdgeTurn}},            //RightGauntlet
                {{REdgeTurn,LEdgeTurn},{REdgeTurn,LEdgeTurn}}};             //RightStart*/

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
                {{RightGauntlet,RightGauntlet},{RightGauntlet,RightGauntlet}}};        //RightStart

            //state data
            bool climbedRamp = false;
            //enum lastIntersection = {LeftGauntlet, LeftCenter, RightCenter, RightCetner};
    };
}