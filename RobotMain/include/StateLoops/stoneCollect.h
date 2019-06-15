#include "StateLoops/MainStates.h"

namespace StateLoops {
    class StoneCollect : public MainStates {
        public:
            void loop();
        private:
            enum stoneCollectStates{} stoneCollectState;
            enum postNumbers{postOne, postTwo, postThree} postNumber;
            void goToPost(enum postNumbers);
            void raiseIntake(enum postNumbers);
            void intakeStone();
            void returnToTape();
    };
}