#ifndef __CONFIGGLOBALSH__
#define __CONFIGGLOBALSH__

#include "libs/Adafruit_SSD1306.h"

extern Adafruit_SSD1306 display;

enum States{lineFollowing,avengerCollecting,avengerScoring,stoneCollecting,stoneScoring,defending};

class MainState{
public:
    static MainState* instance();

    void setState(States newState);
    States getState();

private:
    MainState();
    MainState(const MainState&);
    MainState& operator = (const MainState&);

    static MainState* myInstance;
    States currentState;
};

#endif



