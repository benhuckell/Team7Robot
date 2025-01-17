#include "Functions/debugging.h"
#include "Hardware/HardwareInterface.h"
#include "stateController.h"

extern bool rightStart;

HardwareInterface * HI;
int count = 0;
bool calibrateMode = false;
int QRD_MaxVals[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void debug_setup(){
    HI = HardwareInterface::i();
}

void debugging_loop(){

    digitalWrite(LED_RED, LOW);
    HI->LMotor->setSpeed(0);
    HI->RMotor->setSpeed(0);

    pinMode(PUSH_BUTTON_1, INPUT_PULLDOWN);
    pinMode(PUSH_BUTTON_2, INPUT_PULLDOWN);

    display.clearDisplay();
    display.setCursor(0,0);

    count++;

    display.println(count);
    // display.print("PB1: ");
    // display.println(digitalRead(PUSH_BUTTON_1));
    // display.print("PB2: ");
    // display.println(digitalRead(PUSH_BUTTON_2));

    if(rightStart){
        if(analogRead(CONTROL_POT_1) > 512){
            display.println("Path 534 - Right");
        }
        else{
            display.println("Path 654 - Right");
        }
    }

    else{//leftStart
        if(analogRead(CONTROL_POT_2) > 512){
            display.println("Path 621 - Left");

      }
      else{
          display.println("Path 561 - Left");
      }
    }

    if(digitalRead(PUSH_BUTTON_2) && calibrateMode == false){ 
        delay(1000);
        calibrateMode = true;
    }
    else if(digitalRead(PUSH_BUTTON_2) && calibrateMode == true){
        delay(1000);
        calibrateMode = false;
    }

    if(!calibrateMode){
        display.print(HI->QRD_Vals[0]);
        display.print(" ");
        display.print(HI->QRD_Vals[1]);
        display.print(" ");
        display.print(HI->QRD_Vals[2]);
        display.print(" ");
        display.print(HI->QRD_Vals[3]);
        display.println(" ");
        display.print(HI->QRD_Vals[4]);
        display.print(" ");
        display.print(HI->QRD_Vals[5]);
        display.print(" ");
        display.print(HI->QRD_Vals[6]);
        display.print(" ");
        display.print(HI->QRD_Vals[7]);
        display.println(" ");
    }
    else{
        for(int i = 0;i<8;i++){
            //if we are seeing the white value, print the actual value
            if(HI->QRD_RAW[i] < 75){
                if(QRD_MaxVals[i] >= 75){
                    display.print(QRD_MaxVals[i]);
                }
                else{
                    display.print(HI->QRD_RAW[i]);
                }
                
            }
            else{
                if(HI->QRD_RAW[i]>QRD_MaxVals[i]){
                    QRD_MaxVals[i] = HI->QRD_RAW[i];
                }
                display.print(QRD_MaxVals[i]);
            }
            if(i==3 || i==7){
                display.println(" ");
            }
            else{
                display.print(" ");
            }
            
        } 
    }



    float positionVector[] = { -30.5, -18, -8.4, -1.75, 1.75, 8.4, 18, 30.5 };
    float sum = 0;
    for(int i = 0; i < 8; i++){
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
    HI->update();

    /////ENCODER DEBUG CODE
    //Serial.println("LEN: " + String(HI->LEncoder->getCount()));
    //Serial.println("REN: " + String(HI->REncoder->getCount()));
    //Serial.println("");
    //////////////////

    return;
}

