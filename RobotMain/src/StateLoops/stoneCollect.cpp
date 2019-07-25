#include "StateLoops/stoneCollect.h"
#include "stateController.h"

namespace StateLoops {
    StoneCollect::StoneCollect(){
        StoneCollect::HI = HardwareInterface::i();
    }

    void StoneCollect::loop(){
        display.println("Stone Collecting");
        raiseIntake(Post1);
        intakeStone();
        returnToTape();
    }

    void StoneCollect::raiseIntake(PostNumber postNum) {
        HI->moveIntake(winchTickTargets[postNum]);
    }
    
    void StoneCollect::intakeStone() {
        HI->checkForRock();
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

        ;

    }
}