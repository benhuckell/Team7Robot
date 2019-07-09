#pragma once
#include "Arduino.h"

class Encoder {
    public:
        Encoder(PinName encoder_port_1, PinName encoder_port_2);
        void update();
        int getSpeed(); 
        int getCount();
        void resetCount(); //Be Careful!!!

    private:
        PinName encoder_port_1;
        PinName encoder_port_2;
        int speed;
        int rpm;
        int count;
        float timeSinceLastPulse;
        float timeOfLastPulse;
        const float WheelDiameter = 0.02; //metres
        const int ticksPerRotation = 90;

};