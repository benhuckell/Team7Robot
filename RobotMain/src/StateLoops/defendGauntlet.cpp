#include "StateLoops/defendGauntlet.h"
#include "stateController.h"

using namespace StateLoops;

DefendGauntlet::DefendGauntlet(){
    DefendGauntlet::HI = HardwareInterface::i();
}

void DefendGauntlet::loop(){
    MainState::instance()->setState(lineFollowing);
    display.println("Defending Gauntlet");
    delay(1000);
    return;
}

void DefendGauntlet::defend(){

}
