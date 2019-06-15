#include "StateLoops/MainStates.h"

namespace StateLoops{
    class LineFollow : public MainStates {
        public:
            //LineFollow(Hardware::DriveTrain)
            void loop();
        private:
            enum lineFollowStates{online,offline} lineFollowState;
            void findIR();
            void findPost();
            void findGauntlet();
            void findLine();
    };
}
