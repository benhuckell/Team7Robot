 #include "Functions/lift.h"
 #include "Hardware/HardwareInterface.h"

//#include "stateController.h"
int winchTickTarget;
int currentHeight = 0;

void liftSetup(){
    winchTickTarget = 0;
}

void moveIntake(int abs_target, int speed, int timeout){
    HardwareInterface* HI = HardwareInterface::i(); 
    
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
    Serial.println("WINCH move relativeTar: " + String(relativeTarget));
    Serial.println("WINCH move start: " + String(startingTicks));

    while( abs(HI->WinchEncoder->getCount() - startingTicks) < abs(relativeTarget)){
        delay(10);
    }
    currentHeight = abs_target;
    HI->WinchEncoder->winch_dir = 0;
    HI->pushWinchSpeed(0);
}


void getStone(int winchTickTarget){
    HardwareInterface* HI = HardwareInterface::i();

    //Move claw to stone height
    moveIntake(winchTickTarget,30,10000);
    delay(1000);

    //Close claw
    HI->clawMotor->clawSetPos(300);
    delay(1000);

    //Move claw up to clear stone
    moveIntake(winchTickTarget+40,30,10000);//was +50
    delay(1000);
}