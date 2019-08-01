#include "StateLoops/lineFollow.h"
#include "stateController.h"

using namespace StateLoops;

LineFollow::LineFollow(){
    LineFollow::HI = HardwareInterface::i();
}

void LineFollow::setup(){
    //Turn off LEDs
    digitalWrite(LED_BLUE,LOW);
    digitalWrite(LED_RED,LOW);

    turnStep = 0;
}

void LineFollow::junctionTurn(Turn turn){
    robotSpeed = 30;
    int startTime = millis();
    if(turn == LEdgeTurn){
        while(millis()-startTime < 400){
            followTape(robotSpeed, false, true);//follow right edge
        }
    }
    else if(turn == REdgeTurn){
        while(millis()-startTime < 400){
            followTape(robotSpeed, true, true);//follow right edge
        }
    }
    else if(turn == QRD_Left){
        //QRDTurn(false);//turn left
    }
    else if(turn == QRD_Right){
        //QRDTurn(true);//turn right
    }
    else if(turn == PostTurn){
        stopMoving();
        delay(5000);
    }
}

void LineFollow::loop(){
    robotSpeed = 50;
    int junctionHandling = false;
    followTape(robotSpeed, false,true);
    // if(detectJunction()){
    //     followTape(40,false,true);
    //     // junctionHandling = true;
    //     // junctionTurn(path1[turnStep]);
    // }
    // else{
    //     // if(junctionHandling){
    //     //     junctionHandling = fa lse;
    //     //     turnStep++;
    //     // }
    //     followTape(robotSpeed,false,false);
    // }


    // if(detectJunction()){
    //     digitalWrite(LED_RED,HIGH);
    //     //update position variables
    //     prevPosition = currentPosition;
    //     currentPosition = nextPos;
    //     int angle = nextAngle;
    //     //angle = 70;
    //     bool destinationReached = (currentPosition == destination);
    //     nextPos = nextPosition[currentPosition][dir][destinationReached];
    //     nextAngle = nextTurnAngle[currentPosition][dir][(int)(nextPos == destination)];

    //     //check if current junction is a post
    //     postDetected = (currentPosition <= Post6 && currentPosition >= Post1);

    //     //condition to go to post
    //     if(postDetected && destinationReached){
    //         //find which side the post is on
    //         if(HI->getWeightedError() >= 0){
    //             postOnRight = true;
    //         }else if(HI->getWeightedError() < 0){
    //             postOnRight = false;
    //         }
    //         //drive forwards slightly
    //         int time = millis();
    //         while(millis()-time < 200){
    //             followTape(robotSpeed,true,false);
    //             HI->update();
    //         }
    //         //brake and turn towards post
    //         stopMoving();
    //         if(postOnRight){
    //             HI->turn_single(240, -1, -1, 3000, 0.7);
    //             delay(3000);
    //         }else{
    //             HI->turn_single(240,1,-1,3000,1);
    //             delay(3000);
    //         }
    //         //drive to post
    //         // while(!HI->robotHitPost()){
    //         //     HI->LMotor->setSpeed(50);
    //         //     HI->RMotor->setSpeed(50);
    //         //     HI->LMotor->update();
    //         //     HI->RMotor->update();
    //         // }
    //         stopMoving();
            
    //         for(int i = 0; i < 5; i ++){
    //             digitalWrite(LED_RED, HIGH);
    //             delay(300);
    //             digitalWrite(LED_RED, LOW);
    //             delay(300);
    //         }
    //         MainState::instance()->setState(stoneCollecting);
    //     } 
    //     else{
    //         //turn at junction
    //         stopMoving();
    //         if(angle>0){
    //             HI->turn_single(angle*9, 1, 1, angle*12, 1);
    //         } 
    //         else{
    //             stopMoving();
    //             HI->turn_single(abs(angle)*9, -1, 1, abs(angle)*12, 0.7);
    //         }
    //     }
    //     if(destinationReached){
    //         //update destination
    //         destinationList.pop();
    //         destination = destinationList.front();
    //         //update dir
    //         if(destination <= Post4 || destination >= Post1){
    //             if(startingPosition == LeftGauntlet){
    //                 dir =  CW;
    //             }else{
    //                 dir = CCW;
    //             }
    //         }else if(destination == Post5 || destination == Post6){
    //             if(startingPosition == LeftGauntlet){
    //                 dir = CCW;
    //             }else{
    //                 dir = CW;
    //             }
    //         }
    //     }
    //     HI->update();
    // }
    // else { 
    //     digitalWrite(LED_RED, LOW);
    //     Serial.println("following tape");
    //     followTape(robotSpeed, true,true);
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

float LineFollow::getWeightedEdgeError(bool followRightEdge)
{
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

//runs a PID to follow the tape
void LineFollow::followTape(int robotSpeed, bool followRightEdge, bool edgeFollow){
    float error = 0;

    if(edgeFollow){
        digitalWrite(LED_BLUE,HIGH);
        digitalWrite(LED_RED,LOW);
        error = getWeightedEdgeError(followRightEdge);

        //check for losing line on Left
        // if(followRightEdge){
        //     if(lostLine && error > lineFoundFactor){
        //         lostLine = false;
        //     }
        //     if(lostLine || abs(HI->errorHistory.back() - HI->errorHistory.front()) > lineLostFactor){

        //         error = positionVector[numSensors-1];
        //         lostLine = true;
        //     }
        // }
        // else{ //check for losing line on right
        //     if(lostLine && error < (-1*lineFoundFactor)){
        //         lostLine = false;
        //     }
        //     if(lostLine || abs(HI->errorHistory.back() - HI->errorHistory.front()) > lineLostFactor){
        //         digitalWrite(LED_RED,HIGH);
        //         error = -30;
        //         lostLine = true;
        //     }
        // }

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

    LSpeed = (robotSpeed + speedAdj);
    RSpeed = (robotSpeed - speedAdj);
    // Serial.print(error);
    // Serial.println(speedAdj);
    // Serial.println(LSpeed);
    // Serial.println(RSpeed);
    setMotorSpeeds();

    Serial.println("QRD output: " + String(HI->QRD_Vals[0]) + " " + String(HI->QRD_Vals[1]) + " " + String(HI->QRD_Vals[2]) + " " + String(HI->QRD_Vals[3]) + " " + String(HI->QRD_Vals[4]) + " " + String(HI->QRD_Vals[5]) + " " + String(HI->QRD_Vals[6]) + " " + String(HI->QRD_Vals[7]) + " ");
    Serial.println("Error: " + String(error));

}
//////////////////////////////////////////////////////////////////////////////
///////////////////////////// PID END ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//return true if any sensors detect black
//return false otherwise
bool LineFollow::detectLine(){
    for(int i = 0; i < numSensors; i ++){
        if (HI->QRD_Vals[i] > 0.4){
            return true;
        }
    }
    return false;
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


