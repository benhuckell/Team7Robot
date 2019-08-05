#include "StateLoops/stoneCollect.h"
#include "stateController.h"

namespace StateLoops {
    enum stoneCollectStates{} stoneCollectState;

    StoneCollect::StoneCollect(){
        StoneCollect::HI = HardwareInterface::i();
        nextPostNum = 0;
    }

    void StoneCollect::loop(){        
        Serial.println("Stone Collecting");

        bool rightTurn;
        int stoneHeight;

        if(postList[nextPostNum] == post1 ){
            rightTurn = (HI->team == HI->rightStart);
            stoneHeight = 325;
        }else if(postList[nextPostNum == post2]){

        }else if(postList[nextPostNum == post3]){

        }else if(postList[nextPostNum == post4]){

        }else if(postList[nextPostNum == post5]){

        }else if(postList[nextPostNum == post6]){
            rightTurn = (HI->team == HI->leftStart);
            stoneHeight = 325;
        }
        HI->getStone_const_speed(stoneHeight);

        delay(100);
        Serial.println("start backing out");
        HI->jdubDrive(-1, 50, 40, 32, 3000, 0.4, 0.6, 0);
        delay(1500);
        Serial.println("QRD turn Hardware init");
        HI->QRDTurnNew(rightTurn);
        Serial.println("end turn ");
        delay(1500);
        nextPostNum++;
        MainState::instance()->setState(lineFollowing);
    }

    void StoneCollect::goToPost(enum postNumber) {
        
    }
    
    void StoneCollect::raiseIntake(enum postNumber) {

    }
    
    void StoneCollect::intakeStone() {
        if(HI->timing_flag == false){
            HI->start_time_global = millis();
            HI->timing_flag = true;
        } 

        // HI->clawMotor->clawSetPos(200);
        // delay(1000);
        // HI->clawMotor->clawSetPos(100);
        // delay(1000);


        //moving up to the height of the pole
        if (millis()  - HI->start_time_global < 3000){
            HI-> winchTickTarget=250;
            HI-> Winch_P_gain=1.3;
            HI->WinchEncoder->winch_dir=1;
            HI->moveIntake();
            Serial.println("en: " + String(HI->WinchEncoder->getCount()));
            Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));        
        }
    
        //closing the claw around the rock
        else if (millis()- HI->start_time_global>3000 && millis()- HI->start_time_global<4500){
            HI->clawMotor->clawSetPos(330); //was 300
            delay(1000);
            // HI->clawMotor->clawSetPos(100);
            // delay(3000);
        }

        //lifting up to make sure rock isn't still in the pole mount
        else if(millis()- HI->start_time_global>4500 && millis()- HI->start_time_global<9500){
            HI->WinchEncoder->winch_dir=1;
            HI-> winchTickTarget=270;
            HI-> Winch_P_gain=1;
            HI-> moveIntake();
            Serial.println("en: " + String(HI->WinchEncoder->getCount()));
            Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));
        }

        
        else if (millis()- HI->start_time_global>8500 && millis()- HI->start_time_global<11000){
            HI-> winchTickTarget=100;
            HI->WinchEncoder->winch_dir=-1;
            HI-> Winch_P_gain=1;
            HI-> moveIntake();
            Serial.println("en: " + String(HI->WinchEncoder->getCount()));
            Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));
        // // HI->clawMotor->clawSetPos(100);
        }
        else{

        }
    }
    
    void StoneCollect::returnToTape() {

    }

}

