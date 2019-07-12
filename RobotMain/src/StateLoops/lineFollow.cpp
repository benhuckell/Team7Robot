#include "StateLoops/lineFollow.h"
#include "stateController.h"

using namespace StateLoops;

enum lineFollowStates{online,offline} lineFollowState;

LineFollow::LineFollow(){
    LineFollow::HI = HardwareInterface::i();
}

void LineFollow::loop(){
    followTape(true);
    
}

//update the stored motor speed values
void LineFollow::setMotorSpeeds(){
    
    HI->RMotor->setSpeed(RSpeed);
    HI->LMotor->setSpeed(LSpeed);
    
    // display.println("Rspeed ");
    // display.println(HI->RMotor->getSpeed());
    // display.println("LSpeed ");
    // display.println(HI->LMotor->getSpeed());
}

// Returns the current amount of line following error
float LineFollow::getLinePositionError(bool followRightEdge)
{
    float target;
    float leftEdgeXVal = 0;
    float rightEdgeXVal = 0;
    int i = 0;
    float edgeXPos = 0;
    

    if(followRightEdge){ 
            //Find right edge
        for(i = numSensors-1; (HI->QRD_Vals[i] < HI->QRD_Edge[i]) && (i >= 0); i--){
        }
        //;//intentionally blank for loop
        if(i == numSensors-1){ rightEdgeXVal = 0 + (numSensors-1)*0.5; } //handle case where line is directly below rightmost sensor
        else{
        //interpolate and subtract (numSensors-1)/2 to put a zero x value in the middle of the sensor array
            rightEdgeXVal = i + (HI->QRD_Vals[i] - HI->QRD_Edge[i])/(HI->QRD_Vals[i] - HI->QRD_Vals[i+1]) - (numSensors-1)*0.5;
        }
        edgeXPos = rightEdgeXVal;
        target = 1;
    }
    else{
        //Find left edge
        int i;
        for(i = 0; (HI->QRD_Vals[i] < HI->QRD_Edge[i]) && (i < numSensors); i++)
        ;//intentionally empty for loop
        if(i == 0){ leftEdgeXVal = 0 - (numSensors-1)*0.5; } //handle case where line is directly below leftmost sensor
        else{
        //interpolate and subtract (numSensors-1)/2 to put a zero x value in the middle of the sensor array
            leftEdgeXVal = i - (HI->QRD_Vals[i] - HI->QRD_Edge[i])/(HI->QRD_Vals[i] - HI->QRD_Vals[i-1]) - (numSensors-1)*0.5;
        }
        edgeXPos = leftEdgeXVal;
        target = -1;
    }
    if((rightEdgeXVal - leftEdgeXVal) > POST_TAPE_WIDTH){
        postDetected = true;
    }
    return edgeXPos - target;
}

//runs a PID to follow the tape
void LineFollow::followTape(bool followRightEdge){
    float error = getLinePositionError(followRightEdge); // get current error
    if(postDetected){
        return;
    }
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
    float speedAdj = P_gain*error + I_gain*I_sum + D_gain*D_error;

    if (LSpeed + speedAdj > 100){
        speedAdj = 100-LSpeed;
    }
    else if(LSpeed + speedAdj < -100){
        speedAdj = 0-LSpeed;
    }

    if (RSpeed - speedAdj > 100){
        speedAdj = 100-RSpeed;
    }
    else if (RSpeed - speedAdj < -100){
        speedAdj = 0-RSpeed;
    }
     display.clearDisplay();
     display.print("speedAdj: ");
     display.println(speedAdj);
     display.print("LSpeed: ");
     display.println(LSpeed);
     display.print("RSpeed: ");
     display.println(RSpeed);
    
    LSpeed += speedAdj;
    RSpeed -= speedAdj;
    setMotorSpeeds();
}

void LineFollow::findIR() {

}

void LineFollow::findPost() {

}

void LineFollow::findGauntlet() {

}

void LineFollow::findLine() {

}
