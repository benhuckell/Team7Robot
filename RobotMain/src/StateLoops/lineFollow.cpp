#include "StateLoops/lineFollow.h"
#include "stateController.h"

using namespace StateLoops;



LineFollow::LineFollow(){
    LineFollow::HI = HardwareInterface::i();
}

void LineFollow::loop(){
    followTape(robotSpeed, dir);
 
    //intersection handling
    // if(detectIntersection()){
    //     display.println("INTERSECTION");
    //     intersectionTurn(true);//true for right, false for left
    // }
    // display.println("NOPE");
    
    //bump detection
    // climbedRamp = true;
    // if(climbedRamp && HI->robotWasBumped()){
    //     climbedRamp = true;
    //     HI->LMotor->setSpeed(-20);
    //     HI->RMotor->setSpeed(-20);
    //     HI->LMotor->update();
    //     HI->RMotor->update();
    //     delay(500);
    //     turnXDegrees(180);
    //     robotSpeed = 35;
    //     display.println("BUMPED");
    // } else if(HI->robotHitPost()){
    //     robotSpeed = 0;
    //     MainState::instance()->setState(stoneCollecting);
    // }

    return;
}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////// PID START //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//update the stored motor speed values
void LineFollow::setMotorSpeeds(){
    if(RSpeed < -100) { RSpeed = -100; }
    else if(RSpeed > 100) { RSpeed = 100; }
    if(LSpeed < -100) { LSpeed = -100; }
    else if(LSpeed > 100) { LSpeed = 100; }
    
    HI->LMotor->setSpeed(LSpeed);
    HI->RMotor->setSpeed(RSpeed/straightLineCorrectionFactor);
}

float LineFollow::getWeightedError(){
    float sum = 0;
    bool onBlack = false;
    for(int i = 0; i < numSensors; i++){
        sum += HI->QRD_Vals[i]*positionVector[i];
        if (HI->QRD_Vals[i] > 0.5){
            onBlack = true;
        }
    }
    if(onBlack){
        return sum;
    } else {
        return errorHistory.back();
    }
}

// Returns the current amount of line following error
float LineFollow::getLinePositionError(bool followRightEdge)
{
    float leftEdgeXVal = 0;
    float rightEdgeXVal = 0;
    int i = 0;
    float edgeXPos = 0;

    if(followRightEdge){ 
        //Find right edge
        for(i = numSensors-1; (HI->QRD_Vals[i] < HI->QRD_Thresh[i]) && (i > 0); i--){}//intentionally empty for loop
        //;//intentionally blank for loop
        if(i == numSensors-1){ rightEdgeXVal = 0 + (numSensors-1)*0.5; } //handle case where line is directly below rightmost sensor
        else if(HI->QRD_Vals[i] > HI->QRD_Thresh[i]){
            //interpolate and subtract (numSensors-1)/2 to put a zero x value in the middle of the sensor array
            rightEdgeXVal = (float)i + (float)(HI->QRD_Vals[i] - HI->QRD_Thresh[i])/(HI->QRD_Vals[i] - HI->QRD_Vals[i+1]) - (float)(numSensors-1)*0.5;
        }
        else if(errorHistory.back() > 0){
            rightEdgeXVal = P_gain/10; //get most recent value
        }
        else if(errorHistory.back() < 0){
            rightEdgeXVal = -P_gain/(10*straightLineCorrectionFactor);
        }
        edgeXPos = rightEdgeXVal;
    }
    else{
        //Find left edge
        for(i = 0; (HI->QRD_Vals[i] < HI->QRD_Thresh[i]) && (i < numSensors); i++){}//intentionally empty for loop

        if(i == 0){ leftEdgeXVal = 0 - (numSensors)*0.5; } //handle case where line is directly below leftmost sensor

        else if(HI->QRD_Vals[i] > HI->QRD_Thresh[i]){
        //interpolate and subtract (numSensors-1)/2 to put a zero x value in the middle of the sensor array
            leftEdgeXVal = (float)i - (float)(HI->QRD_Vals[i] - HI->QRD_Thresh[i])/(HI->QRD_Vals[i] - HI->QRD_Vals[i-1]) - (float)(numSensors-1)*0.5;
        }
        else{
            leftEdgeXVal = errorHistory.back(); //get most recent value
        }
        edgeXPos = leftEdgeXVal;
    }
    return edgeXPos;
}

//runs a PID to follow the tape
void LineFollow::followTape(int robotSpeed, bool followRightEdge){
    P_gain = analogRead(CONTROL_POT_1)/10;
    D_gain = analogRead(CONTROL_POT_2)/2;

    float error = getWeightedError();

    errorHistory.push(error); // add current error to errorQueue
    if(errorHistory.size() > ERROR_HISTORY_SIZE){ // keep queue size at ERROR_HISTORY_SIZE
        errorHistory.pop();
    }

    I_sum += error;
    if(I_sum > maxISum){ //set to max sum to avoid integral windup
        I_sum = maxISum;
    }

    //calc average derivative
    float D_error = (errorHistory.back() - errorHistory.front())/(ERROR_HISTORY_SIZE);

    //adjust speed of both wheels to correct for error
    float speedAdj = 0;
    speedAdj = P_gain*error + I_gain*I_sum + D_gain*D_error;

    LSpeed = (robotSpeed + speedAdj)/straightLineCorrectionFactor;
    RSpeed = (robotSpeed - speedAdj);
    setMotorSpeeds();
}
//////////////////////////////////////////////////////////////////////////////
///////////////////////////// PID END ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void LineFollow::findIR() {

}

void LineFollow::findGauntlet() {

}

void LineFollow::turnOnLine(){
    int start = millis();
    while(!detectLine()){
        HI->LMotor->setSpeed(20);
        HI->RMotor->setSpeed(-20);
        HI->LMotor->update();
        HI->RMotor->update();
        if(millis() - start > 3000){
            return;
        }
    }
}

//+ve for CW -ve for CCW
void LineFollow::turnXDegrees(int angle){
    int startCountR = HI->REncoder->getCount();
    HI->LMotor->setSpeed(20);
    HI->RMotor->setSpeed(-20);
    HI->LMotor->update();
    HI->RMotor->update();
    while(HI->REncoder->getCount()-startCountR < ticksPerAngle*angle){
    }
}

//return true if any sensors detect black
//return false otherwise
bool LineFollow::detectLine(){
    for(int i = 0; i < numSensors; i ++){
        if (HI->QRD_Vals[i] > 0.5){
            return true;
        }
    }
    return false;
}

//return true if line array detects two separate lines
//return false otherwise
bool LineFollow::detectIntersection(){
    int count = 0;
    bool firstLineFound = false;
    bool middleFound = false;
    bool secondLineFound = false;
    for(int i = 0; i < numSensors; i ++){
        if (HI->QRD_Vals[i] > HI->QRD_Thresh[i]){
            count++;
            firstLineFound = true;
            if(middleFound == true){
                secondLineFound = true;
            }
        }else if(firstLineFound){
            middleFound = true;
        }
    }
    if(count >= 4 && secondLineFound){//line found if we found a black then white then black sensor and at least 4 sensors were black
        return true;
    }
    return false;
}

//turn at an intersection
void LineFollow::intersectionTurn(bool dir){ //true for right, false for left
    if(dir){
        turnXDegrees(20);
    }else{
        turnXDegrees(-20);
    }
}

bool LineFollow::detectJunction(){
    int count = 0;
    for(int i = 0; i < numSensors; i ++){
        if (HI->QRD_Vals[i] > 0.4){
            count++;
        }
    }
    if(count >= 4){
        return true;
    }
    return false;
}

void LineFollow::turnTowardsPost() {
    if(clockwiseMove){
        turnXDegrees(-90);
    }else{
        turnXDegrees(90);
    }
}

void LineFollow::stopRobot(){
    HI->LMotor->setSpeed(0);
    HI->RMotor->setSpeed(0);
    HI->LMotor->update();
    HI->RMotor->update();
    delay(500);
}

bool LineFollow::getNextTurnAngle(Position lastPosition, Position destination){
    //if(clockwiseMove)
}