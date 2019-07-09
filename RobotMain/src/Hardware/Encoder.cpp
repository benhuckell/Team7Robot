#include "Hardware/Encoder.h"
    
Encoder::Encoder(PinName encoder_port_1, PinName encoder_port_2) {
    Encoder::encoder_port_1 = encoder_port_1;
    Encoder::encoder_port_2 = encoder_port_2;
}

//gets called on every rising edge of the encoder output
void Encoder::update_port_1() {
    if(millis() - timeOfLastPulse_2 < timeOfLastPulse_2 - timeOfLastPulse_1){ //if encoder 'slot' is the leading one
        direction = 1;
    }
    timeSinceLastPulse_1 = timeOfLastPulse_1 - millis();
    timeOfLastPulse_1 = millis();

    
    count++;
    float ticksPerSecond = 1/timeSinceLastPulse_1; 
    float rotationsPerSecond = ticksPerSecond/ticksPerRotation;
    rpm = rotationsPerSecond*60;
    speed = rotationsPerSecond*PI*WheelDiameter;direction;
}

void Encoder::update_port_2() {
    if(millis() - timeOfLastPulse_2 > timeOfLastPulse_2 - timeOfLastPulse_1){ //if encoder 'slot' is the trailing one
        direction = 1;
    }
    timeSinceLastPulse_2 = timeOfLastPulse_2 - millis();
    timeOfLastPulse_2 = millis();

    direction = 1;
    count++;
    float ticksPerSecond = 1/timeSinceLastPulse_2; 
    float rotationsPerSecond = ticksPerSecond/ticksPerRotation;
    rpm = rotationsPerSecond*60;
    speed = rotationsPerSecond*PI*WheelDiameter*direction;

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
