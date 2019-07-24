#include "StateLoops/debugging.h"
#include "stateController.h"

using namespace StateLoops;

int count = 0;

Debugging::Debugging(){
    Debugging::HI = HardwareInterface::i();
}

void Debugging::loop(){
    HI->LMotor->setSpeed(0);
    HI->RMotor->setSpeed(0);
    
    pinMode(PUSH_BUTTON_1, INPUT_PULLDOWN);
    pinMode(PUSH_BUTTON_2, INPUT_PULLDOWN);

    display.clearDisplay();
    display.setCursor(0,0);

    count++;

    display.println(count);
    display.print("PB1: ");
    display.println(digitalRead(PUSH_BUTTON_1));
    display.print("PB2: ");
    display.println(digitalRead(PUSH_BUTTON_2));

    display.print(HI->QRD_RAW[0]);
    display.print(" ");
    display.print(HI->QRD_RAW[1]);
    display.print(" ");
    display.print(HI->QRD_RAW[2]);
    display.print(" ");
    display.print(HI->QRD_RAW[3]);
    display.println(" ");
    display.print(HI->QRD_RAW[4]);
    display.print(" ");
    display.print(HI->QRD_RAW[5]);
    display.print(" ");
    display.print(HI->QRD_RAW[6]);
    display.print(" ");
    display.print(HI->QRD_RAW[7]);
    display.println(" ");

    //HardwareInterface::i()->update();

    display.display();

    return;
}
