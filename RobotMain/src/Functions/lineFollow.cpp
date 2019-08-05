#include "Functions/lineFollow.h"
//#include "stateController.h"
#include "Hardware/HardwareInterface.h"
#include "Functions/drive.h"

float I_sum = 0; //cummulative sum

void followTape(int robotSpeed, bool followRightEdge, bool edgeFollow){
    HardwareInterface* HI = HardwareInterface::i();
    float error = 0;

    if(edgeFollow){
        digitalWrite(LED_BLUE,HIGH);
        digitalWrite(LED_RED,LOW);
        error = getWeightedEdgeError(followRightEdge);

    }
    else{
        digitalWrite(LED_BLUE,LOW);
        digitalWrite(LED_RED,HIGH);
        error = HI->getWeightedError();
    }

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

    if(edgeFollow){
        speedAdj = P_gain_edge*error + D_gain_edge*D_error;
    }
    else{
        speedAdj = P_gain*error + I_gain*I_sum + D_gain*D_error;
    }

    int LSpeed = (robotSpeed + speedAdj);
    int RSpeed = (robotSpeed - speedAdj);
    
    // Serial.println(speedAdj);
    // Serial.println(LSpeed);
    // Serial.println(RSpeed);
    HI->pushDriveSpeeds(LSpeed, RSpeed/straightLineCorrectionFactor);

    //Serial.println("QRD output: " + String(HI->QRD_Vals[0]) + " " + String(HI->QRD_Vals[1]) + " " + String(HI->QRD_Vals[2]) + " " + String(HI->QRD_Vals[3]) + " " + String(HI->QRD_Vals[4]) + " " + String(HI->QRD_Vals[5]) + " " + String(HI->QRD_Vals[6]) + " " + String(HI->QRD_Vals[7]) + " ");
    //Serial.println("Error: " + String(error));

}

float getWeightedEdgeError(bool followRightEdge){
    HardwareInterface* HI = HardwareInterface::i();
    float weightedSum = 0;
    bool onBlack = false;
    int maxIndex = -1;
    if(followRightEdge){
        for(int i = numSensors-1; i >= 0; i--){
            if(HI->QRD_Vals[i] > QRD::QRD_Thresh){
                onBlack = true;
                if(i == 0){
                    for(int i = numSensors-1; i >= 0; i--){
                        weightedSum += HI->QRD_Vals[i]*positionVector[i];
                    }
                    break;
                }
                else if(HI->QRD_Vals[i-1] < HI->QRD_Vals[i]){
                    maxIndex = i;
                    for(int i = maxIndex-1; i < numSensors; i++){
                        weightedSum += HI->QRD_Vals[i]*positionVector[i];
                    }
                    break;
                }
            }
        }
    }
    else{//left edge
        for(int i = 0; i < numSensors; i++){
            if(HI->QRD_Vals[i] > QRD::QRD_Thresh){
                onBlack = true;
                if(i == numSensors-1){
                    for(int i = 0; i < numSensors; i++){
                        weightedSum += HI->QRD_Vals[i]*positionVector[i];
                    }
                    break;
                }
                else if(HI->QRD_Vals[i+1] < HI->QRD_Vals[i]){
                    maxIndex = i;
                    for(int i = maxIndex+1; i >= 0; i--){
                        weightedSum += HI->QRD_Vals[i]*positionVector[i];
                    }
                    break;
                }
            }
        }
    }


    if(onBlack){
        return weightedSum;
    } else if(HI->errorHistory.back() < 0){
        return positionVector[0];
    } else {
        return positionVector[numSensors-1];
    }

}

void lineFollowSetup(){
    //Turn off LEDs
    digitalWrite(LED_BLUE,LOW);
    digitalWrite(LED_RED,LOW);

}


void LEdgeTurn(){
    int startTime = millis();
    HardwareInterface* HI = HardwareInterface::i();
    while(millis()-startTime < 350){
        followTape(ROBOTSPEED, false, true);//follow left edge
        HI->update();
    }
}

void REdgeTurn(){
    int startTime = millis();
    HardwareInterface* HI = HardwareInterface::i();
    while(millis()-startTime < 350){
            followTape(ROBOTSPEED, true, true);//follow right edge
            HI->update();
        }
}


void Post1Turn(){
    //first post, 
    int startTime = millis();
    HardwareInterface* HI = HardwareInterface::i();  
    stopMoving();
    delay(1000);
    turn_single_constant(-71, 10000,40);
    delay(3000);
    HI->update();

    //Drive to post
    drive_stop_seq(1,2500,25,0,38);

    delay(2000);
}

void Post2Turn(){
        //first post, 
    int startTime = millis();
    HardwareInterface* HI = HardwareInterface::i();  
    stopMoving();
    delay(1000);
    turn_single_constant(84, 10000,40);
    delay(3000);
    HI->update();

    //Drive to post
    drive_stop_seq(1,2500,25,0,38);

    delay(2000);
    }

bool detectJunction(){
    HardwareInterface* HI = HardwareInterface::i();
    int count = 0;
    for(int i = 0; i < numSensors; i ++){
        if (HI->QRD_Vals[i] > 0.7){
            count++;
        }
    }
    if(count >= 4){
        return true;
    }
    return false;
}
