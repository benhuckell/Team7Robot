#pragma once
#ifndef LINEFOLLOW_H
#define LINEFOLLOW_H
void followTape(int robotSpeed, bool followRightEdge, bool edgeFollow);

#define ROBOTSPEED 30

//constant data
float const P_gain = 1.5; // K_p
float const I_gain = 0; // K_i
float const D_gain = 105;//14; // K_d 
float const P_gain_edge = 2.1;
float const D_gain_edge = 150;
static const int numSensors = 8;
float const positionVector[numSensors] = { -30.5 ,-18.0 ,-8.4, -1.75, 1.75, 8.4, 18.0, 30.5 };
const float maxISum = 2; //max sum to avoid integral windup
const unsigned int ERROR_HISTORY_SIZE = 2; //max size of error queue
const float straightLineCorrectionFactor = 1.35;


float getWeightedEdgeError(bool followRightEdge);
void lineFollowSetup();

//junctions
void LEdgeTurn();
void REdgeTurn();
void QRD_Left();
void QRD_Right();
void PostTurnLeft();
void PostTurnRight(); 

bool detectJunction();

#endif