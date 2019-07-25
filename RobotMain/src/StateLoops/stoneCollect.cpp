#include "StateLoops/stoneCollect.h"
#include "stateController.h"

namespace StateLoops {
    StoneCollect::StoneCollect(){
        StoneCollect::HI = HardwareInterface::i();
    }

    void StoneCollect::loop(){
        display.println("Stone Collecting");
    }

    void StoneCollect::raiseIntake(PostNumber postNum) {
        HI->moveIntake(winchTickTargets[postNum]);
    }
    
    void StoneCollect::intakeStone() {

    }
    
    void StoneCollect::returnToTape() {

    }
}