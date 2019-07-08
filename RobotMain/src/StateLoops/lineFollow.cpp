#include "Arduino.h"
#include "StateLoops/lineFollow.h"
#include <Adafruit_SSD1306.h>
#include "configGlobals.h"

namespace StateLoops {
    

    enum lineFollowStates{online,offline} lineFollowState;
    
    void LineFollow::loop(){
        //MainState::i()->setState(avengerCollecting);
        display.println("Line Following");
        //delay(1000);

        return;
    }
    
    void LineFollow::findIR() {

    }

    void LineFollow::findPost() {

    }

    void LineFollow::findGauntlet() {

    }

    void LineFollow::findLine() {

    }
}