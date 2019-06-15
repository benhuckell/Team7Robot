#include "StateLoops/MainStates.h"

namespace StateLoops {
    class DefendGauntlet : public MainStates {
        public:
            void loop();
        private:
            void defend();
    };
}