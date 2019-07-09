#include "Hardware/Encoder.h"
#include <ctime>
    
Encoder::Encoder(PinName encoder_port_1, PinName encoder_port_2) {
    Encoder::encoder_port_1 = encoder_port_1;
    Encoder::encoder_port_2 = encoder_port_2;
}

//gets called on every rising edge of the encoder output
void Encoder::update() {
    timeSinceLastPulse = timeOfLastPulse - std::clock();
    timeOfLastPulse = std::clock();

    count++;
    float ticksPerSecond = 1/timeSinceLastPulse; 
    float rotationsPerSecond = ticksPerSecond/ticksPerRotation;
    rpm = rotationsPerSecond*60;
    speed = rotationsPerSecond*PI*WheelDiameter;
}

int Encoder::getSpeed() {
    return speed;
}

int Encoder::getCount() {
    return count;
}

void Encoder::resetCount() {
    count = 0;
}
