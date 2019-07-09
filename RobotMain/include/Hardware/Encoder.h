#pragma once
#include "Arduino.h"

class Encoder {
    public:
        Encoder(PinName encoder_port_1, PinName encoder_port_2);
        void update_port_1();
        void update_port_2();
        int getSpeed(); 
        int getCount();
        void resetCount(); //Be Careful!!!

    private:
        PinName encoder_port_1;
        PinName encoder_port_2;
        unsigned int timeSinceLastPulse_1;
        unsigned int timeOfLastPulse_1;
        unsigned int timeSinceLastPulse_2;
        unsigned int timeOfLastPulse_2;
        int speed;
        int rpm;
        int count;
        const float WheelDiameter = 0.02; //metres
        const int ticksPerRotation = 90;

};