 namespace StateLoops {
    class MainStates {
        public:
            void loop();
    };

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

    class AvengerCollect : public MainStates {
        public:
            void loop();
        private:
            enum avengerCollectStates{} avengerCollectState;
            void intakeAvengers();
    };

    class AvengerScore : public MainStates {
        public:
            void loop();
        private:
            enum avengerScoreStates{} avengerScoreState;
            void goToBeacon();
            void dumpAvengers();
    };

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

    class Defend : public MainStates {
        public:
            void loop();
        private:
            void defend();
    };
 }