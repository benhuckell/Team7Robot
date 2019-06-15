#include "StateLoops/MainStates.h"
namespace StateLoops {
    class AvengerScore : public MainStates {
        public:
            void loop();
        private:
            enum avengerScoreStates{} avengerScoreState;
            void goToBeacon();
            void dumpAvengers();
    };
}