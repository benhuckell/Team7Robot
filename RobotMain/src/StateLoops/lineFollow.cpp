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
    junctionHandling = false;
}

void LineFollow::junctionTurn(Turn turn){
    Serial.println("junction");
    robotSpeed = 30;
    int startTime = millis();
    if(turn == LEdgeTurn){
        while(millis()-startTime < 250){
            followTape(robotSpeed, false, true);//follow left edge
            HI->update();
        }
    }
    else if(turn == REdgeTurn){
        while(millis()-startTime < 250){
            followTape(robotSpeed, true, true);//follow right edge
            HI->update();
        }
    }
    else if(turn == QRD_Left){
        while(millis()-startTime < 250){
            followTape(robotSpeed, true, true);//follow right edge
            HI->update();
        }
        QRDTurn(false);//turn left
    }
    else if(turn == QRD_Right){
        while(millis()-startTime < 250){
            followTape(robotSpeed, false, true);//follow left edge
            HI->update();
        }
        QRDTurn(true);//turn right
    }
    else if(turn == PostTurnLeft){//Left post
        goForwardsSlightly(350, robotSpeed, false);
        slewBrake(robotSpeed, 100, -5);
        delay(1000);
        HI->turn_single_constant(-190, 10000, 40);
        delay(3000);
        HI->update();

        //Drive to post
        drive_stop_seq(1,2500,25,0,40);

        stopMoving();
        delay(5000);
    }
    else if(turn == PostTurnRight){//Right post
        goForwardsSlightly(500, robotSpeed, true);
        slewBrake(robotSpeed, 100, -5);
        delay(1000);
        HI->turn_single_constant(175,3000,40);
        delay(6000);
        HI->update();
        // stopMoving();
        // delay(5000);
    }
}

void LineFollow::loop(){
    //robotSpeed = 43;
    // HI->LMotor->setSpeed(50);
    // HI->RMotor->setSpeed(50/straightLineCorrectionFactor);

    //followTape(robotSpeed,false,false);
    
    HI->clawMotor->clawSetPos(300);
    delay(1000);
    HI->clawMotor->clawSetPos(200);
    delay(1000);

    // if(detectJunction()){
    //     //followTape(40,false,true);
    //     junctionHandling = true;
    //     junctionTurn(path1[turnStep]);
    // }
    // else{
    //     if(junctionHandling){
    //         Serial.println(turnStep);
    //         junctionHandling = false;
    //         turnStep++;
    //     }
    //     followTape(robotSpeed,false,false);
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
    // Serial.println(speedAdj);
    // Serial.println(LSpeed);
    // Serial.println(RSpeed);
    setMotorSpeeds();

    //Serial.println("QRD output: " + String(HI->QRD_Vals[0]) + " " + String(HI->QRD_Vals[1]) + " " + String(HI->QRD_Vals[2]) + " " + String(HI->QRD_Vals[3]) + " " + String(HI->QRD_Vals[4]) + " " + String(HI->QRD_Vals[5]) + " " + String(HI->QRD_Vals[6]) + " " + String(HI->QRD_Vals[7]) + " ");
    //Serial.println("Error: " + String(error));

}
//////////////////////////////////////////////////////////////////////////////
///////////////////////////// PID END ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void LineFollow::drive_stop(int direction, int timeout, float delta_trip, float kdrift, int maxpower){
    
    //must set 
    //delta trip is speed of encoder to trip
    delay(30); //needed to accumlate encoder tics for speed
    if(!drive_stop_START_TIME_INIT){
        Serial.println("init of stop");
        drive_stop_START_TIME = millis();
        digitalWrite(LED_BLUE,LOW);
        drive_stop_START_TIME_INIT = true;
    }

    float ave_speed = (abs(HI->LEncoder->getSpeed()) + abs(HI->REncoder->getSpeed()))/2;
    int net_time = millis() - drive_stop_START_TIME;
    int drift = HI->LEncoder->getCount() - HI->REncoder->getCount();

    Serial.println("ave speed" + String(ave_speed));

    if(net_time < 400){
        //get moving for some small time
        HI->LMotor->setSpeed( (direction*maxpower) - (drift * kdrift));
        HI->RMotor->setSpeed( (direction*maxpower) + (drift * kdrift)/1.45);
    }
    else if((ave_speed < delta_trip) || (net_time > timeout)){
        Serial.println("exit reached!");
        //exit: detected post
        HI->LMotor->setSpeed(0);
        HI->RMotor->setSpeed(0);
        HI->LMotor->update();
        HI->RMotor->update();

        //change state here
        MainState::instance()->setState(stoneCollecting);

        //reset START_TIME_INIT
        drive_stop_START_TIME_INIT = false;
        digitalWrite(LED_BLUE,HIGH); // for testing
    }
    else{
        //continue moving in dir
        Serial.println("continue in dir");
        HI->LMotor->setSpeed( (direction*maxpower) - (drift * kdrift));
        HI->RMotor->setSpeed( (direction*maxpower) + (drift * kdrift));
    }

  }

void LineFollow::drive_stop_seq(int direction, int timeout, float delta_trip, float kdrift, int maxpower){
    
    //must set 
    //delta trip is speed of encoder to trip

    int drift;
    float ave_speed;

    int start_time = millis();
    while(millis() - start_time < 800){
        HI->LMotor->setSpeed( (direction*maxpower) );
        HI->RMotor->setSpeed( (direction*maxpower) );
        HI->update();
        ave_speed = (abs(HI->LEncoder->getSpeed()) + abs(HI->REncoder->getSpeed()))/2;
        drift = HI->LEncoder->getCount() - HI->REncoder->getCount();
    }

    while((ave_speed > delta_trip) && ((millis()-start_time) < timeout)){

    ave_speed = (abs(HI->LEncoder->getSpeed()) + abs(HI->REncoder->getSpeed()))/2;
    drift = HI->LEncoder->getCount() - HI->REncoder->getCount();

    Serial.println("ave speed" + String(ave_speed));
    HI->LMotor->setSpeed( (direction*maxpower) - (drift * kdrift));
    HI->RMotor->setSpeed( (direction*maxpower) + (drift * kdrift)/1.45);
    HI-> update();
    delay(30);
    }
    Serial.println("exit reached!");

    HI->LMotor->setSpeed(0);
    HI->RMotor->setSpeed(0);
    HI->LMotor->update();
    HI->RMotor->update();

    MainState::instance()->setState(stoneCollecting);

  }






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
        if (HI->QRD_Vals[i] > 0.8){
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


void LineFollow::slewBrake(int startSpeed, int duration, int targetSpeed){
    int startTime = millis();
    int time_elapsed = startTime;
    while(time_elapsed-startTime < duration){
        HI->LMotor->setSpeed(startSpeed*(1-float(time_elapsed-startTime)/float(duration)) + targetSpeed*(float(time_elapsed-startTime)/float(duration)));
        HI->RMotor->setSpeed(startSpeed*(1-float(time_elapsed-startTime)/float(duration)) + targetSpeed*(float(time_elapsed-startTime)/float(duration)));
        HI->LMotor->update();
        HI->RMotor->update();
        time_elapsed = millis();
    }
    HI->LMotor->setSpeed(0);
    HI->RMotor->setSpeed(0);
    HI->LMotor->update();
    HI->RMotor->update();
}

void LineFollow::goForwardsSlightly(int targetTicks, int robotSpeed, bool postOnRight){
    int LStartTicks = HI->LEncoder->getCount();
    int RStartTicks = HI->REncoder->getCount();

    HI->LMotor->setSpeed(robotSpeed);
    HI->RMotor->setSpeed(robotSpeed);
    HI->LMotor->update();
    HI->RMotor->update();
    delay(50);

    while(HI->LEncoder->getCount() + HI->REncoder->getCount() - LStartTicks - RStartTicks < targetTicks){
        followTape(robotSpeed, !postOnRight, false);
        HI->update();
    }
}

void LineFollow::QRDTurn(bool turnRight){
    if(turnRight){
        while(HI->errorHistory.back() - HI->errorHistory.front() < 10.0){
            HI->LMotor->setSpeed(30);
            HI->RMotor->setSpeed(-30);
            HI->update();
            HI->errorHistory.push(HI->getWeightedError());
            HI->errorHistory.pop();
        }
    }
    else{//turn left
        while(HI->errorHistory.back() - HI->errorHistory.front() > -10.0){
            digitalWrite(LED_BLUE, HIGH);
            HI->LMotor->setSpeed(-30);
            HI->RMotor->setSpeed(30);
            HI->update();
            HI->errorHistory.push(HI->getWeightedError());
            HI->errorHistory.pop();
        }
        digitalWrite(LED_BLUE,LOW);
    }
}