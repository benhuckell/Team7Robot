#include "StateLoops/MainStates.h"

namespace StateLoops {
    class StoneScore : public MainStates {
        public:
            void loop();
        private:
            enum stoneScoreStates{} stoneScoreState;
            enum holeNumbers{holeOne,holeTwo,holeThree,holeFour,holeFive} holeNumber;
            void goToGauntlet();
            void findHole(enum holeNumbers);
            void placeStone();
            void returnToTape();
    };
}