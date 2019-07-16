#include "StateLoops/lineFollow.h"
#include "stateController.h"

using namespace StateLoops;

enum lineFollowStates{online,offline} lineFollowState;

LineFollow::LineFollow(){
    LineFollow::HI = HardwareInterface::i();
}

void LineFollow::loop(){
    int robotSpeed = 50;
    display.println("Line Following");
    followTape(robotSpeed, true);
    return;
}

//update the stored motor speed values
void LineFollow::setMotorSpeeds(){
    
    if(RSpeed < -100) { RSpeed = -100; }
    else if(RSpeed > 100) { RSpeed = 100; }
    if(LSpeed < -100) { LSpeed = -100; }
    else if(LSpeed > 100) { LSpeed = 100; }

    HI->RMotor->setSpeed(RSpeed);
    HI->LMotor->setSpeed(LSpeed);
    
    display.print("RSpeed: ");
    display.println(HI->RMotor->getSpeed());
    display.print("LSpeed: ");
    display.println(HI->LMotor->getSpeed());
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
        target = 0.5;
        //Find right edge
        for(i = numSensors-1; (HI->QRD_Vals[i] < HI->QRD_Edge[i]) && (i > 0); i--){}//intentionally empty for loop
        
        // display.print("i: ");
        // display.println(i);
        //;//intentionally blank for loop
        if(i == numSensors-1){ rightEdgeXVal = 0 + (numSensors-1)*0.5; } //handle case where line is directly below rightmost sensor
        else if(HI->QRD_Vals[i] > HI->QRD_Edge[i]){
            //interpolate and subtract (numSensors-1)/2 to put a zero x value in the middle of the sensor array
            rightEdgeXVal = (float)i + (float)(HI->QRD_Vals[i] - HI->QRD_Edge[i])/(HI->QRD_Vals[i] - HI->QRD_Vals[i+1]) - (float)(numSensors-1)*0.5;
            // display.print("inter: ");
            // display.println((float)(HI->QRD_Vals[i] - HI->QRD_Edge[i])/(HI->QRD_Vals[i] - HI->QRD_Vals[i+1]));
        }
        else{
            rightEdgeXVal = errorHistory.back() + target; //get most recent value
        }
        edgeXPos = rightEdgeXVal;

        display.print(HI->QRD_Vals[0]);
        display.print(" ");
        display.print(HI->QRD_Vals[1]);
        display.print(" ");
        display.print(HI->QRD_Vals[2]);
        display.print(" ");
        display.print(HI->QRD_Vals[3]);
        display.println(" ");
    }
    else{
        target = -0.5;
        //Find left edge
        for(i = 0; (HI->QRD_Vals[i] < HI->QRD_Edge[i]) && (i < numSensors); i++){}//intentionally empty for loop

        if(i == 0){ leftEdgeXVal = 0 - (numSensors)*0.5; } //handle case where line is directly below leftmost sensor

        else if(HI->QRD_Vals[i] > HI->QRD_Edge[i]){
        //interpolate and subtract (numSensors-1)/2 to put a zero x value in the middle of the sensor array
            leftEdgeXVal = (float)i - (float)(HI->QRD_Vals[i] - HI->QRD_Edge[i])/(HI->QRD_Vals[i] - HI->QRD_Vals[i-1]) - (float)(numSensors-1)*0.5;
        }
        else{
            rightEdgeXVal = errorHistory.back() + target; //get most recent value
        }
        edgeXPos = leftEdgeXVal;
    }
    if((rightEdgeXVal - leftEdgeXVal) > POST_TAPE_WIDTH){
        postDetected = true;
    }
    return edgeXPos - target;
}

//runs a PID to follow the tape
void LineFollow::followTape(int robotSpeed, bool followRightEdge){
    display.clearDisplay();
    float error = getLinePositionError(followRightEdge); // get current error
    
    // display.print("error: ");
    // display.println(error);

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
    float speedAdj = 0;
    speedAdj = P_gain*error + I_gain*I_sum + D_gain*D_error;
    // display.print("Padj: ");
    // display.println(P_gain*error);
    
    // display.print("Iadj: ");
    // display.println(I_gain*I_sum);
    
    // display.print("Dadj: ");
    // display.println(D_gain*D_error);
    
    
    // display.print("speedAdj: ");
    // display.println(speedAdj);
    // display.print("LSpeed: ");
    // display.println(LSpeed);
    // display.print("RSpeed: ");
    // display.println(RSpeed);

    LSpeed = robotSpeed + speedAdj;
    RSpeed = robotSpeed - speedAdj;
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
