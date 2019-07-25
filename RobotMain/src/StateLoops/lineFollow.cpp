#include "StateLoops/lineFollow.h"
#include "stateController.h"

using namespace StateLoops;

LineFollow::LineFollow(){
    LineFollow::HI = HardwareInterface::i();

    //Calculate paths
    if(startingPosition == LeftGauntlet){ //team == THANOS
        for(int i = 0; i < 6; i++){
            CurrentPath.push(PostPriority[i]);
            CurrentPath.push(LeftIntersection);
            CurrentPath.push(LeftGauntlet);
        }
    }else{ //team == METHANOS
        for(int i = 0; i < 6; i++){
            CurrentPath.push(PostPriority[i]);
            CurrentPath.push(RightIntersection);
            CurrentPath.push(RightGauntlet);
        }
    }
    destination = CurrentPath.front();

    nextPos = nextPosition[currentPosition][dir][currentPosition == destination];
    nextAngle = nextTurnAngle[currentPosition][dir][(int)(nextPos == destination)];

    if(destination <= Post4 && destination >= Post1){
        if(startingPosition == LeftGauntlet){
            dir =  CW;
        }else{
            dir = CCW;
        }
    }else if(destination == Post5 || destination == Post6){
        if(startingPosition == LeftGauntlet){
            dir = CCW;
        }else{
            dir = CW;
        }
    }
}

void LineFollow::loop(){
    int robotSpeed = 60;
    bool postOnRight = true; //true for right false for left
    if(detectJunction()){
        //update position variables
        prevPosition = currentPosition;
        currentPosition = nextPos;
        int angle = nextAngle;
        bool destinationReached = (currentPosition == destination);
        nextPos = nextPosition[currentPosition][dir][destinationReached];
        nextAngle = nextTurnAngle[currentPosition][dir][(int)(nextPos == destination)];

        //check if current junction is a post
        postDetected = (destination <= Post6 && destination >= Post1);

        //condition to go to post
        if(postDetected && destinationReached){
            //find which side the post is on
            if(getWeightedError() >= 0){
                postOnRight = true;
            }else if(getWeightedError() < 0){
                postOnRight = false;
            }
            //drive forwards slightly
            int time = millis();
            while(millis()-time < 150){
                followTape(robotSpeed,true);
                HI->update();
            }
            //brake and turn towards post
            stopMoving();
            if(postOnRight){
                turnXDegrees(90);
                delay(3000);
            }else{
                turnXDegrees(-90);
                delay(3000);
            }
            //drive to post
            while(!HI->robotHitPost()){
                HI->LMotor->setSpeed(50);
                HI->RMotor->setSpeed(50);
                HI->LMotor->update();
                HI->RMotor->update();
            }
            stopMoving();
            MainState::instance()->setState(stoneCollecting);
        } 
        else{
            //turn at junction
            turnXDegrees(angle);
            delay(3000);
        }
        if(destinationReached){
            //update destination
            CurrentPath.pop();
            destination = CurrentPath.front();
            //update dir
            if(destination <= Post4 || destination >= Post1){
                if(startingPosition == LeftGauntlet){
                    dir =  CW;
                }else{
                    dir = CCW;
                }
            }else if(destination == Post5 || destination == Post6){
                if(startingPosition == LeftGauntlet){
                    dir = CCW;
                }else{
                    dir = CW;
                }
            }
        }
    }
    else if(HI->robotWasBumped()){
        //turn around

        //update PostPriority list
    }
    else { 
        followTape(robotSpeed, true);
    }
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
    } else if(errorHistory.back() < 0){
        return positionVector[0];
    } else {
        return positionVector[numSensors-1];
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
    P_gain = (float)analogRead(CONTROL_POT_1)/200.0;
    D_gain = (float)analogRead(CONTROL_POT_2)/100.0;

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

//+ve for CW -ve for CCW
void LineFollow::turnXDegrees(int angle){
    int startRCount = HI->REncoder->getCount();
    int startLCount = HI->LEncoder->getCount();
    bool LStopped = false;
    bool RStopped = false;
    if(angle > 0){
        HI->LMotor->setSpeed(0);
        HI->RMotor->setSpeed(-70/straightLineCorrectionFactor);
        HI->update();
    }else if(angle < 0){
        HI->LMotor->setSpeed(-70);
        HI->RMotor->setSpeed(40/straightLineCorrectionFactor);
        HI->update();
    }
    while(!LStopped || !RStopped){
        if(HI->REncoder->getCount()-startRCount > ticksPerAngle*abs(angle)){
            HI->RMotor->setSpeed(0);
            HI->RMotor->update();
            LStopped = true;
        }
        if(HI->LEncoder->getCount()-startLCount > ticksPerAngle*abs(angle)){
            HI->LMotor->setSpeed(0);
            HI->LMotor->update();
            RStopped = true;
        }
    }
    

    HI->update();
    return;
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

//turn at an intersection
void LineFollow::intersectionTurn(){
    if(dir == CW){
        turnXDegrees(20);
    }else{
        turnXDegrees(-20);
    }
}

bool LineFollow::detectJunction(){
    int count = 0;
    for(int i = 0; i < numSensors; i ++){
        if (HI->QRD_Vals[i] > 0.5){
            count++;
        }
    }
    if(count >= 4){
        return true;
    }
    return false;
}

void LineFollow::turnTowardsPost() {
    if(dir){
        turnXDegrees(-90);
    }else{
        turnXDegrees(90);
    }
}

void LineFollow::stopMoving(){
    HI->RMotor->setSpeed(-50);
    HI->LMotor->setSpeed(-50);
    HI->update();
    delay(150);
    HI->RMotor->setSpeed(0);
    HI->LMotor->setSpeed(0);
    HI->update();
    delay(150);
}

void LineFollow::turnOnLine(){
    HI->RMotor->setSpeed(-50);
    HI->LMotor->setSpeed(50);
    HI->RMotor->update();
    HI->LMotor->update();

    while(detectLine()){}
    while(!detectLine()){}
    
    HI->RMotor->setSpeed(0);
    HI->LMotor->setSpeed(0);
    HI->RMotor->update();
    HI->LMotor->update();
}