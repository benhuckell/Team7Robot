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
    int robotSpeed = 43;
    bool postOnRight = true; //true for right' false for left
    followTape(robotSpeed,false,true);
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
       for(int i = numSensors-1; i > 0; i--){
           if(HI->QRD_Vals[i] > QRD::QRD_Thresh){
               onBlack = true;
               if(i == 0){
                   maxIndex = 0;
                   for(int i = 0; i < numSensors; i++){
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
       return 0;
   }
   else{//left edge
       for(int i = 0; i < numSensors; i++){
           if(HI->QRD_Vals[i] > QRD::QRD_Thresh){
               onBlack = true;
               if(i == numSensors-1){
                   maxIndex = 0;
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


// Returns the current amount of line following error
float LineFollow::getLinePositionError(bool followRightEdge)
{
   float QRD_Thresh = 0.4;
   float returnError = 0.0;
   float maxVal = 0;
   int maxIndex = 0;
   float secondMaxVal = 0;
   int secondMaxIndex = 0;
   if(followRightEdge){
       for(int i = numSensors-1; i > 0; i--){
           if(HI->QRD_Vals[i] > QRD_Thresh){
               if(HI->QRD_Vals[i-1] < HI->QRD_Vals[i]){
                   maxVal = HI->QRD_Vals[i];
                   maxIndex = i;
                   if(i == numSensors-1){
                       secondMaxIndex = numSensors-2;
                   }
                   else{
                       //secondMaxIndex = (HI->QRD_Vals[i+1] > HI->QRD_Vals[i-1]) ? i+1 : i-1;
                       Serial.println("QRD output: " + String(HI->QRD_Vals[0]) + " " + String(HI->QRD_Vals[1]) + " " + String(HI->QRD_Vals[2]) + " " + String(HI->QRD_Vals[3]) + " " + String(HI->QRD_Vals[4]) + " " + String(HI->QRD_Vals[5]) + " " + String(HI->QRD_Vals[6]) + " " + String(HI->QRD_Vals[7]) + " ");
                       Serial.print("maxIndex: " + String(maxIndex));
                       return (positionVector[maxIndex]+((HI->QRD_Vals[maxIndex+1])/(HI->QRD_Vals[maxIndex]+HI->QRD_Vals[maxIndex+1]))*(positionVector[maxIndex+1]-positionVector[maxIndex])-((HI->QRD_Vals[maxIndex+1])/(HI->QRD_Vals[maxIndex]+HI->QRD_Vals[maxIndex+1]))*(positionVector[maxIndex]-positionVector[maxIndex-1]));
                   }
                   secondMaxVal = HI->QRD_Vals[secondMaxIndex];
                   break;
               }
               else if(i == 1){
                   maxVal = HI->QRD_Vals[0];
                   maxIndex = 0;
                   secondMaxIndex = 1;
                   secondMaxVal = HI->QRD_Vals[secondMaxIndex];
                   break;
               }
           }
       }
   }else{//left edge
       for(int i = 0; i < numSensors-1; i++){
           if(HI->QRD_Vals[i] > QRD_Thresh){
               if(HI->QRD_Vals[i+1] < HI->QRD_Vals[i]){
                   maxVal = HI->QRD_Vals[i];
                   maxIndex = i;
                   if(i == 0){
                       secondMaxIndex = 1;
                   }
                   else{
                       //secondMaxIndex = (HI->QRD_Vals[i+1] > HI->QRD_Vals[i-1]) ? i+1 : i-1;
                       Serial.println(String(HI->QRD_Vals[0]) + " " + String(HI->QRD_Vals[1]) + " " + String(HI->QRD_Vals[2]) + " " + String(HI->QRD_Vals[3]) + " " + String(HI->QRD_Vals[4]) + " " + String(HI->QRD_Vals[5]) + " " + String(HI->QRD_Vals[6]) + " " + String(HI->QRD_Vals[7]) + " ");
                       Serial.print(maxIndex);
                       return (positionVector[maxIndex]+((HI->QRD_Vals[maxIndex+1])/(HI->QRD_Vals[maxIndex]+HI->QRD_Vals[maxIndex+1]))*(positionVector[maxIndex+1]-positionVector[maxIndex])-((HI->QRD_Vals[maxIndex+1])/(HI->QRD_Vals[maxIndex]+HI->QRD_Vals[maxIndex+1]))*(positionVector[maxIndex]-positionVector[maxIndex-1]));
                   }
               }
               else if(i == numSensors-2){
                   maxVal = HI->QRD_Vals[numSensors-1];
                   maxIndex = numSensors-1;
                   secondMaxIndex = numSensors-2;
                   secondMaxVal = HI->QRD_Vals[secondMaxIndex];
                   break;
               }
           }
       }
   }
   if(maxVal == 0 || secondMaxVal == 0){
       if(HI->errorHistory.back()<0){
           returnError = -30.5;
       }
       else if(HI->errorHistory.back()>0){
           returnError = 30.5;
       }
       else{
           returnError = 0;
       }
   }
   else{
       //Interpolate to find error, need to always find left most point and add
       if(maxIndex < secondMaxIndex){ //if max index is less than second max index
           returnError = positionVector[maxIndex] + ((secondMaxVal)/(maxVal+secondMaxVal))*(positionVector[secondMaxIndex]-positionVector[maxIndex]);
       }
       else{ //if max index is greater than second max index
           returnError = positionVector[secondMaxIndex] + ((maxVal)/(maxVal+secondMaxVal))*(positionVector[maxIndex]-positionVector[secondMaxIndex]);
       }
   }
 
 
 
   return returnError;
}


//runs a PID to follow the tape
void LineFollow::followTape(int robotSpeed, bool followRightEdge, bool edgeFollow){
    float error = 0;

    if(edgeFollow){
        error = getWeightedEdgeError(followRightEdge);
        digitalWrite(LED_BLUE,HIGH);
    }
    else{
        digitalWrite(LED_RED,HIGH);
        error = HI->getWeightedError();
    }
    Serial.println("error: " + String(error));

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

bool LineFollow::detectJunction(){
    digitalWrite(LED_BLUE,HIGH);
    int count = 0;
    for(int i = 0; i < numSensors; i ++){
        if (HI->QRD_Vals[i] > 0.65){
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