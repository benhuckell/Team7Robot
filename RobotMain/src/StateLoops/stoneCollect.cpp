#include "StateLoops/stoneCollect.h"
#include "stateController.h"

namespace StateLoops {
    enum stoneCollectStates{} stoneCollectState;
    enum postNumbers{postOne, postTwo, postThree} postNumber;
    
    StoneCollect::StoneCollect(){
        StoneCollect::HI = HardwareInterface::i();
    }

    void StoneCollect::loop(){
        display.println("Stone Collecting");
    }

    void StoneCollect::goToPost(enum postNumbers) {
        
    }
    
    void StoneCollect::raiseIntake(enum postNumbers) {

    }
    
    void StoneCollect::intakeStone() {

    }
    
    void StoneCollect::returnToTape() {
        MainState::instance()->setState(lineFollowing);
    }
}