#include "StateLoops/MainStates.h"
namespace StateLoops {
    class AvengerCollect : public MainStates {
        public:
            void loop();
        private:
            enum avengerCollectStates{} avengerCollectState;
            void intakeAvengers();
    };
}