#include "StateLoops/stoneCollect.h"
#include "stateController.h"

namespace StateLoops {
    StoneCollect::StoneCollect(){
        StoneCollect::HI = HardwareInterface::i();
    }

    void StoneCollect::loop(){
        display.println("Stone Collecting");
        HI->raiseIntake(winchTickTargets[HI->postNum] - HI->WinchEncoder->getCount());
        HI->getStone();
        returnToTape();
    }
    
    void StoneCollect::returnToTape() {
        //back up
        HI->LMotor->setSpeed(-40);
        HI->RMotor->setSpeed(-40);
        HI->LMotor->update();
        HI->RMotor->update();
        delay(400);
        HI->LMotor->setSpeed(0);
        HI->RMotor->setSpeed(0);
        HI->LMotor->update();
        HI->RMotor->update();

        HI->turnOnLine(HI->dir);
    }
}