#include "StateLoops/defendGauntlet.h"
#include "stateController.h"

using namespace StateLoops;
void DefendGauntlet::loop(){
    MainState::instance()->setState(lineFollowing);
    display.println("Defending Gauntlet");
    delay(1000);
    return;
}

void DefendGauntlet::defend(){

}
