#ifndef __CONFIGGLOBALSH__
#define __CONFIGGLOBALSH__

#include "Adafruit_SSD1306.h"
#include "FreeMono9pt7b.h"

extern Adafruit_SSD1306 display;

//State::StateClasses[] = {lineFollow, avengerCollect};

enum States{lineFollowing,avengerCollecting,avengerScoring,stoneCollecting,stoneScoring,defending};

class MainState{
public:
    static MainState* i();

    void setState(States newState);
    States getState();

private:
    MainState();
    MainState(const MainState&);
    MainState& operator=(const MainState&);

    static MainState* myInstance;
    States currentState;
};

#endif



