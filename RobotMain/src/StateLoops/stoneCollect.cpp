#include "StateLoops/stoneCollect.h"
#include "stateController.h"
 

using namespace StateLoops;

enum stoneCollectStates{} stoneCollectState;
enum postNumbers{postOne, postTwo, postThree} postNumber;

StoneCollect::StoneCollect(){
    StoneCollect::HI = HardwareInterface::i();
}

void StoneCollect::loop(){
    display.println("Stone Collecting");//should be on post
}

void StoneCollect::goToPost(postNumbers) {
    
}

void StoneCollect::returnToTape() {
    MainState::instance()->setState(lineFollowing);
}
