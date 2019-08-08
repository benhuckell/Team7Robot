#pragma once
#ifndef LINEFOLLOW_H
#define LINEFOLLOW_H
void followTape(int robotSpeed, bool followRightEdge, bool edgeFollow);

#define ROBOTSPEED 40

//constant data
float const P_gain = 1.6; // K_p
float const I_gain = 0; // K_i
float const D_gain = 110;//14; // K_d 
float const P_gain_edge = 2.55;
float const D_gain_edge = 165;
static const int numSensors = 8;
float const positionVector[numSensors] = { -30.5 ,-18.0 ,-8.4, -1.75, 1.75, 8.4, 18.0, 30.5 };
const float maxISum = 2; //max sum to avoid integral windup
const unsigned int ERROR_HISTORY_SIZE = 2; //max size of error queue
const float straightLineCorrectionFactor = 1.11;



float getWeightedEdgeError(bool followRightEdge);
void lineFollowSetup();

//junctions
void LEdgeTurn();
void REdgeTurn();
void Post1Turn(bool rightStart);
void Post2Turn(bool rightStart);
void Post3Turn(bool rightStart);
void Post4Turn(bool rightStart); 
void Post5Turn(bool rightStart); 
void Post6Turn(bool rightStart);

bool detectJunction(float thres = 0.8);

bool detectBumpInRoad(float thres);
#endif