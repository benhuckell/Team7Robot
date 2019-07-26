#include "StateLoops/lineFollow.h"
#include "stateController.h"

using namespace StateLoops;

LineFollow::LineFollow(){
    LineFollow::HI = HardwareInterface::i();
}

void LineFollow::setup(){
    //Calculate paths
    currentPosition = startingPosition;
    if(startingPosition == LeftStart){ //team == THANOS
        for(int i = 0; i < 6; i++){
            destinationList.push(PostPriority[i]);
            destinationList.push(LeftIntersection);
            destinationList.push(LeftGauntlet);
        }
    }else{ //team == METHANOS
        for(int i = 0; i < 6; i++){
            destinationList.push(PostPriority[i]);
            destinationList.push(RightIntersection);
            destinationList.push(RightGauntlet);
        }
    }
    destination = destinationList.front();

    if(destination <= Post4 && destination >= Post1){
        if(startingPosition == LeftStart){
            dir =  CW;
        }else{
            dir = CCW;
        }
    }else if(destination == Post5 || destination == Post6){
        if(startingPosition == LeftStart){
            dir = CCW;
        }else{
            dir = CW;
        }
    }
    Serial.print(dir);
    nextPos = nextPosition[currentPosition][dir][currentPosition == destination];
    nextAngle = nextTurnAngle[currentPosition][dir][(int)(nextPos == destination)];

}
void LineFollow::loop(){
    //setMotorSpeeds();
    int robotSpeed = 60;
    bool postOnRight = true; //true for right false for left
    if(detectJunction()){
        digitalWrite(LED_RED,HIGH);
        //update position variables
        prevPosition = currentPosition;
        currentPosition = nextPos;
        int angle = nextAngle;
        //angle = 70;
        bool destinationReached = (currentPosition == destination);
        nextPos = nextPosition[currentPosition][dir][destinationReached];
        nextAngle = nextTurnAngle[currentPosition][dir][(int)(nextPos == destination)];

        //check if current junction is a post
        postDetected = (currentPosition <= Post6 && currentPosition >= Post1);

        //condition to go to post
        if(postDetected && destinationReached){
            //find which side the post is on
            if(HI->getWeightedError() >= 0){
                postOnRight = true;
            }else if(HI->getWeightedError() < 0){
                postOnRight = false;
            }
            //drive forwards slightly
            int time = millis();
            while(millis()-time < 240){
                followTape(robotSpeed,true);
                HI->update();
            }
            //brake and turn towards post
            stopMoving();
            if(postOnRight){
                HI->turn_single(240, -1, -1, 3000, 0.7);
                delay(3000);
            }else{
                HI->turn_single(240,1,-1,3000,1);
                delay(3000);
            }
            //drive to post
            // while(!HI->robotHitPost()){
            //     HI->LMotor->setSpeed(50);
            //     HI->RMotor->setSpeed(50);
            //     HI->LMotor->update();
            //     HI->RMotor->update();
            // }
            stopMoving();
            
            for(int i = 0; i < 5; i ++){
                digitalWrite(LED_RED, HIGH);
                delay(300);
                digitalWrite(LED_RED, LOW);
                delay(300);
            }
            MainState::instance()->setState(stoneCollecting);
        } 
        else{
            //turn at junction
            if(angle>0){
                HI->turn_single(angle*9, 1, 1, angle*12, 1);
            } 
            else{
                digitalWrite(LED_BLUE,HIGH);
                HI->turn_single(abs(angle)*9, -1, 1, abs(angle)*12, 0.7);
            }
        }
        if(destinationReached){
            //update destination
            destinationList.pop();
            destination = destinationList.front();
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
        HI->update();
    }
    else { 
        digitalWrite(LED_RED, LOW);
        Serial.println("following tape");
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
        else if(HI->errorHistory.back() > 0){
            rightEdgeXVal = P_gain/10; //get most recent value
        }
        else if(HI->errorHistory.back() < 0){
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
            leftEdgeXVal = HI->errorHistory.back(); //get most recent value
        }
        edgeXPos = leftEdgeXVal;
    }
    return edgeXPos;
}

//runs a PID to follow the tape
void LineFollow::followTape(int robotSpeed, bool followRightEdge){
    nextTurnAngle[LeftGauntlet][CW][false] = analogRead(CONTROL_POT_1)/10.0;
    nextTurnAngle[LeftGauntlet][CCW][false] = analogRead(CONTROL_POT_1)/10.0;
    nextTurnAngle[LeftIntersection][CCW][false] = analogRead(CONTROL_POT_2)/10.0;

    float error = HI->getWeightedError();

    HI->errorHistory.push(error); // add current error to errorQueue
    if(HI->errorHistory.size() > ERROR_HISTORY_SIZE){ // keep queue size at ERROR_HISTORY_SIZE
        HI->errorHistory.pop();
    }

    I_sum += error;
    if(I_sum > maxISum){ //set to max sum to avoid integral windup
        I_sum = maxISum;
    }

    //calc average derivative
    float D_error = (HI->errorHistory.back() - HI->errorHistory.front())/(ERROR_HISTORY_SIZE);

    //adjust speed of both wheels to correct for error
    float speedAdj = 0;
    speedAdj = P_gain*error + I_gain*I_sum + D_gain*D_error;

    LSpeed = (robotSpeed + speedAdj);
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

//return true if any sensors detect black
//return false otherwise
bool LineFollow::detectLine(){
    for(int i = 0; i < numSensors; i ++){
        if (HI->QRD_Vals[i] > 0.6){
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
        if (HI->QRD_Vals[i] > 0.75){
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
    HI->RMotor->setSpeed(-35);
    HI->LMotor->setSpeed(-35);
    HI->update();
    delay(200);
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