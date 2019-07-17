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
    display.print("K_p: ");
    display.println(analogRead(CONTROL_POT_1));
    display.print("K_d: ");
    display.println(analogRead(CONTROL_POT_2));

    display.print(HI->QRD_Vals[0]);
    display.print(" ");
    display.print(HI->QRD_Vals[1]);
    display.print(" ");
    display.print(HI->QRD_Vals[2]);
    display.print(" ");
    display.print(HI->QRD_Vals[3]);
    display.println(" ");

    float positionVector[4] = { -7.3, -1.25, 1.25, 7.3 };
    float sum = 0;
    for(int i = 0; i < 4; i++){
        sum += HI->QRD_Vals[i]*positionVector[i];
    }
    
    display.print(sum);
    display.println(" ");
    // display.print(float(HI->QRD_Vals[0] - HI->QRD_Min[0])/float(HI->QRD_Max[0] - HI->QRD_Min[0]));
    // display.print(" ");
    // display.print(float(HI->QRD_Vals[1] - HI->QRD_Min[1])/float(HI->QRD_Max[1] - HI->QRD_Min[1]));
    // display.print(" ");
    // display.print(float(HI->QRD_Vals[2] - HI->QRD_Min[2])/float(HI->QRD_Max[2] - HI->QRD_Min[2]));
    // display.print(" ");
    // display.print(float(HI->QRD_Vals[3] - HI->QRD_Min[3])/float(HI->QRD_Max[3] - HI->QRD_Min[3]));
    // display.print(" ");

    //HardwareInterface::i()->update();

    display.display();

    return;
}

