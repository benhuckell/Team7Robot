#include "stateController.h"

MainState* MainState::myInstance = NULL;

MainState::MainState():
    currentState(lineFollowing){ 
}

MainState::MainState(const MainState& t){

}

MainState& MainState::operator=(const MainState& t){
    
}

MainState* MainState::i(){
    if (myInstance == NULL){
        myInstance = new MainState(); 
        return myInstance;
    }
    return myInstance;
}

void MainState::setState(States newState){
    currentState = newState;
}

States MainState::getState(){
    return currentState;
}