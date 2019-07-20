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

    display.clearDisplay();
    display.setCursor(0,0);

    count++;

    display.println(count);
    Serial.print(count);
    
    // pinMode(PUSH_BUTTON_1, INPUT_PULLDOWN);
    // pinMode(PUSH_BUTTON_2, INPUT_PULLDOWN);
    // display.print("PB1: ");
    // display.println(digitalRead(PUSH_BUTTON_1));
    // display.print("PB2: ");
    // display.println(digitalRead(PUSH_BUTTON_2));

    // display.print("K_p: ");
    // display.println(analogRead(CONTROL_POT_1));
    // display.print("K_d: ");
    // display.println(analogRead(CONTROL_POT_2));

    // display.print(HI->QRD_Vals[0]);
    // display.print(" ");
    // display.print(HI->QRD_Vals[1]);
    // display.print(" ");
    // display.print(HI->QRD_Vals[2]);
    // display.print(" ");
    // display.print(HI->QRD_Vals[3]);
    // display.println(" ");
    // display.print(HI->QRD_Vals[4]);
    // display.print(" ");
    // display.print(HI->QRD_Vals[5]);
    // display.print(" ");
    // display.print(HI->QRD_Vals[6]);
    // display.print(" ");
    // display.print(HI->QRD_Vals[7]);
    // display.println(" ");

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

    // display.print("RSpeed: ");
    // display.println(HI->REncoder->getSpeed());
    // if(HI->robotWasBumped()){
    //     display.println("BUMPED");
    // }
    // else{
    //     display.println("NOPE");
    // }
    
    int numBlack = 0;
    bool firstLineFound = false;
    bool middleFound = false;
    bool secondLineFound = false;
    for(int i = 0; i < numSensors; i ++){
        display.print(HI->QRD_Vals[i]);
        display.print(" ");
        if (HI->QRD_Vals[i] > 0.5){
            numBlack++;
            firstLineFound = true;
            if(middleFound == true){
                secondLineFound = true;
            }
        }else if(firstLineFound){
            middleFound = true;
        }
    }
    display.print(numBlack);
    display.println(" ");
    if(numBlack >= 4 && secondLineFound){//line found if we found a black then white then black sensor and at least 4 sensors were black
        display.println("INTERSECTION");
    }
    display.println("NOPE");
    // float positionVector[numSensors] = { -30.5 ,-18.0 ,-8.4, -1.75, 1.75, 8.4, 18.0, 30.5 };
    // float sum = 0;
    // for(int i = 0; i < numSensors; i++){
    //     sum += HI->QRD_Vals[i]*positionVector[i];
    // }
    // display.print(sum);
    // display.println(" ");

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
