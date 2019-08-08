 #include "Functions/lift.h"
 #include "Hardware/HardwareInterface.h"

//#include "stateController.h"
int winchTickTarget;
int currentHeight = 35;

void liftSetup(){
    winchTickTarget = 0;
}

void moveIntake(int abs_target, int speed, int timeout){
    HardwareInterface* HI = HardwareInterface::i(); 
    int startTime = millis();
    //target 
    int relativeTarget = abs_target - currentHeight;
    int startingTicks = HI->WinchEncoder->getCount();
    
    int direction;

    if(relativeTarget > 0){
        direction = 1;
        //move up
    }
    else{
        //move down
        direction = -1;
    }

    HI->WinchEncoder->winch_dir = -direction;


    HI->pushWinchSpeed(speed*direction);
    Serial.println("Winch speed: " + String(speed*direction));
    Serial.println("WINCH move relativeTar: " + String(relativeTarget));
    Serial.println("WINCH move start: " + String(startingTicks));
    Serial.println("encdoer cout RAW: " + String(HI->WinchEncoder->getCount()));
    while(( abs(HI->WinchEncoder->getCount() - startingTicks) < abs(relativeTarget))&& (millis() - startTime < timeout)){
        Serial.println("moving");
        delay(10);
        Serial.println("encdoer cout RAW: " + String(HI->WinchEncoder->getCount()));
    }

    Serial.println("Exit loop");
    currentHeight = abs_target;
    
    if(direction == -1){
        HI->pushWinchSpeed(14);
    }
    else{
        HI->pushWinchSpeed(7);
    }
    delay(20);
    HI->WinchEncoder->winch_dir = 0;
}

void getStone(int winchTickTarget, int winchTickTargetWinch_2){
    HardwareInterface* HI = HardwareInterface::i();

    //Move claw to stone height
    moveIntake(winchTickTarget,95,5000);
    delay(1000);

    //Close claw
    HI->clawMotor->clawSetPos(210);
    delay(1000);

    //Move claw up to clear stone
    moveIntake(winchTickTargetWinch_2,45,3000);
    delay(1000);
}