#ifndef __CONFIGH__
#define __CONFIGH__

#include "Adafruit_SSD1306.h"
#include "StateLoops/avengerCollect.h"
#include "StateLoops/avengerScore.h"
#include "StateLoops/defendGauntlet.h"
#include "StateLoops/lineFollow.h"
#include "StateLoops/stoneCollect.h"
#include "StateLoops/stoneScore.h"
#include "StateLoops/State.h"

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



