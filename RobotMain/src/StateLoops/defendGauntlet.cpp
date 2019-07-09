#include "StateLoops/defendGauntlet.h"
#include "Arduino.h"
#include "stateController.h"

namespace StateLoops {
    void DefendGauntlet::loop(){
        MainState::i()->setState(lineFollowing);
        display.println("Defending Gauntlet");
        delay(1000);
        return;
    }

    void DefendGauntlet::defend(){

    }
}