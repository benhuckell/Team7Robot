#include "StateLoops/stoneCollect.h"
#include "stateController.h"

namespace StateLoops {
    enum stoneCollectStates{} stoneCollectState;
    enum postNumbers{postOne, postTwo, postThree} postNumber;

    StoneCollect::StoneCollect(){
        StoneCollect::HI = HardwareInterface::i();
    }
    
    void StoneCollect::loop(){
        intakeStone();
    }

    void StoneCollect::goToPost(enum postNumbers) {
        
    }
    
    void StoneCollect::raiseIntake(enum postNumbers) {

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
            HI->clawMotor->clawSetPos(300);
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

    void StoneCollect::getStone_const_speed(){
        HI-> winchTickTarget = 250;
        HI->WinchEncoder->winch_dir=1;

        //raise intake
        while(HI->winchTickTarget - HI->WinchEncoder->getCount() > 5){
            HI->moveIntake_const_speed();
            Serial.println("en: " + String(HI->WinchEncoder->getCount()));
            Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));
        }

        //closing the claw around the rock
        HI->clawMotor->clawSetPos(300);
        delay(1000);
        
        HI->WinchEncoder->winch_dir=1;
        HI-> winchTickTarget=270;

        //lifting up to make sure rock isn't still in the pole mount
        while(HI->winchTickTarget - HI->WinchEncoder->getCount() > 5){
            HI-> moveIntake_const_speed();
            Serial.println("en: " + String(HI->WinchEncoder->getCount()));
            Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));
        }
    }
}