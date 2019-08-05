 #include "Functions/lift.h"
 #include "Hardware/HardwareInterface.h"

//#include "stateController.h"
int winchTickTarget;

void liftSetup(){
    winchTickTarget = 0;
}
void moveIntake() {
    HardwareInterface* HI = HardwareInterface::i(); 
    //set the height that the winch raises the entire assembly to
    //need to use PID to get to the correct height
    int tick_num=HI->WinchEncoder->getCount();
    int tickError=winchTickTarget-tick_num;

    float WinchSpeed=0;
    WinchSpeed = Winch_P_gain*tickError;

    //to set what happens at edge cases
    if(WinchSpeed>60){
        WinchSpeed=60;
    }
    if(WinchSpeed<-60){
        WinchSpeed=-60;
    }
    HI->pushWinchSpeed(WinchSpeed);
}

void moveIntake_const_speed(){
    HardwareInterface* HI = HardwareInterface::i(); 
    //Serial.println("Encoder: Winch: " + String(WinchEncoder->getCount()));
    int WinchSpeed = 40;

    HI->WinchMotor->setSpeed(WinchSpeed);
    //Serial.println("speed:" + String(WinchSpeed));
    HI->WinchMotor->update();
    HI->WinchEncoder->update();
}

void getStone_const_speed(){
        HardwareInterface* HI = HardwareInterface::i(); 
        winchTickTarget = 320;
        HI->WinchEncoder->winch_dir=-1;

        int startingTicks = HI->WinchEncoder->getCount();

        //raise intake1
        while((HI->WinchEncoder->getCount() - startingTicks) < winchTickTarget){
            moveIntake_const_speed();
            //Serial.println("en: " + String(HI->WinchEncoder->getCount()));
            //Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));
        }

        Serial.println("Done first up");

        HI->WinchMotor->setSpeed(0);
        HI->WinchMotor->update();

        //closing the claw around the rock

        int startClawTime = millis();
        while(millis()- startClawTime < 1400){
            HI->clawMotor->clawSetPos(200);
        }

        Serial.println("Closed claw");

        delay(1000);

        HI->WinchEncoder->winch_dir=-1;
        winchTickTarget=380;


        //lifting up to make sure rock isn't still in the pole mount
        while((HI->WinchEncoder->getCount() - startingTicks) < winchTickTarget){
            moveIntake_const_speed();
            Serial.println("en: " + String(HI->WinchEncoder->getCount()));
            Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));
        }

        Serial.println("Done");

        HI->WinchMotor->setSpeed(0);
        HI->WinchMotor->update();
    }


