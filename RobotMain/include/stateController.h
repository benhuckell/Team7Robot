#ifndef __CONFIGGLOBALSH__
#define __CONFIGGLOBALSH__

#include "libs/Adafruit_SSD1306.h"
// #include "Functions/lineFollow.h"
// #include "Functions/mix.h"
// #include "Functions/debugging.h"
// #include "Functions/drive.h"
// #include "Functions/lift.h"


// #include "Hardware/ports.h"
// #include "Functions/debugging.h"
// #include "Paths/Path1.h"
// #include "Hardware/HardwareInterface.h"


extern Adafruit_SSD1306 display;

enum States{lineFollowing,avengerCollecting,avengerScoring,stoneCollecting,stoneScoring,defending,debugging};

class MainState{
public:
    static MainState* instance();

    void setState(States newState);
    States getState();
    HardwareInterface* HI;

private:
    MainState();
    MainState(const MainState&);
    MainState& operator = (const MainState&);

    static MainState* myInstance;
    States currentState;

};

#endif



