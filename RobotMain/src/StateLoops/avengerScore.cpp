#include "StateLoops/avengerScore.h"
#include "Arduino.h"
#include "config.h"

namespace StateLoops {

    enum avengerScoreStates{} avengerScoreState;

    void AvengerScore::loop(){
        MainState::i()->setState(stoneCollecting);
        display.println("Avenger Scoring");
        delay(1000);
        return;
    }

    void AvengerScore::goToBeacon() {

    }
    void AvengerScore::dumpAvengers() {

    }
}