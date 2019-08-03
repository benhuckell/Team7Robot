#include "StateLoops/debugging.h"
#include "stateController.h"

using namespace StateLoops;

int count = 0;
bool calibrateMode = false;
int QRD_MaxVals[8] = {0, 0, 0, 0, 0, 0, 0, 0};

Debugging::Debugging(){
    Debugging::HI = HardwareInterface::i();
}

void Debugging::loop(){
    HI->LMotor->setSpeed(0);
    HI->RMotor->setSpeed(0);
    Serial.println("debugging");

    // HI->WinchMotor->setSpeed(80);
    // HI->WinchMotor->update();
    // Serial.println(HI->WinchEncoder->getCount());
    if(HI->timing_flag == false){
        HI->start_time_global = millis();
        HI->timing_flag = true;
    } 

    // HI->clawMotor->clawSetPos(200);
    // delay(1000);
    // HI->clawMotor->clawSetPos(100);
    // delay(1000);


    //moving up to the height of the pole
    if (millis()  - HI->start_time_global < 3000){
        HI-> winchTickTarget=250;
        HI-> Winch_P_gain=1.3;
        HI->WinchEncoder->winch_dir=1;
        HI->moveIntake();
        Serial.println("en: " + String(HI->WinchEncoder->getCount()));
        Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));        
    }
   
    //closing the claw around the rock
    else if (millis()- HI->start_time_global>3000 && millis()- HI->start_time_global<4500){
        HI->clawMotor->clawSetPos(300);
        delay(1000);
        // HI->clawMotor->clawSetPos(100);
        // delay(3000);
    }

    //lifting up to make sure rock isn't still in the pole mount
    else if(millis()- HI->start_time_global>4500 && millis()- HI->start_time_global<9500){
        HI->WinchEncoder->winch_dir=1;
        HI-> winchTickTarget=270;
        HI-> Winch_P_gain=1;
        HI-> moveIntake();
        Serial.println("en: " + String(HI->WinchEncoder->getCount()));
        Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));
    }

    
    else if (millis()- HI->start_time_global>8500 && millis()- HI->start_time_global<11000){
        HI-> winchTickTarget=100;
        HI->WinchEncoder->winch_dir=-1;
        HI-> Winch_P_gain=1;
        HI-> moveIntake();
        Serial.println("en: " + String(HI->WinchEncoder->getCount()));
        Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));
    // // HI->clawMotor->clawSetPos(100);
    }
    else{

    }


    // HI->winchTickTarget=20;
    // HI->moveIntake();
    // delay(1000);

    // digitalWrite(LED_RED, LOW);
    // HI->LMotor->setSpeed(0);
    // HI->RMotor->setSpeed(0);

    // pinMode(PUSH_BUTTON_1, INPUT_PULLDOWN);
    // pinMode(PUSH_BUTTON_2, INPUT_PULLDOWN);

    // display.clearDisplay();
    // display.setCursor(0,0);

    // count++;

    // display.println(count);
    // display.print("PB1: ");
    // display.println(digitalRead(PUSH_BUTTON_1));
    // display.print("PB2: ");
    // display.println(digitalRead(PUSH_BUTTON_2));
    // display.print("turn1: ");
    // display.println(float(analogRead(CONTROL_POT_1))/float(10.0));
    // display.print("turn2: ");
    // display.println(float(analogRead(CONTROL_POT_2))/float(10.0));

    // if(digitalRead(PUSH_BUTTON_2) && calibrateMode == false){ 
    //     delay(1000);
    //     calibrateMode = true;
    // }
    // else if(digitalRead(PUSH_BUTTON_2) && calibrateMode == true){
    //     delay(1000);
    //     calibrateMode = false;
    // }

    // if(!calibrateMode){
    //     display.print(HI->QRD_Vals[0]);
    //     display.print(" ");
    //     display.print(HI->QRD_Vals[1]);
    //     display.print(" ");
    //     display.print(HI->QRD_Vals[2]);
    //     display.print(" ");
    //     display.print(HI->QRD_Vals[3]);
    //     display.println(" ");
    //     display.print(HI->QRD_Vals[4]);
    //     display.print(" ");
    //     display.print(HI->QRD_Vals[5]);
    //     display.print(" ");
    //     display.print(HI->QRD_Vals[6]);
    //     display.print(" ");
    //     display.print(HI->QRD_Vals[7]);
    //     display.println(" ");
    // }
    // else{
    //     for(int i = 0;i<8;i++){
    //         //if we are seeing the white value, print the actual value
    //         if(HI->QRD_RAW[i] < 75){
    //             if(QRD_MaxVals[i] >= 75){
    //                 display.print(QRD_MaxVals[i]);
    //             }
    //             else{
    //                 display.print(HI->QRD_RAW[i]);
    //             }
                
    //         }
    //         else{
    //             if(HI->QRD_RAW[i]>QRD_MaxVals[i]){
    //                 QRD_MaxVals[i] = HI->QRD_RAW[i];
    //             }
    //             display.print(QRD_MaxVals[i]);
    //         }
    //         if(i==3 || i==7){
    //             display.println(" ");
    //         }
    //         else{
    //             display.print(" ");
    //         }
            
    //     } 
    // }



    // float positionVector[] = { -30.5, -18, -8.4, -1.75, 1.75, 8.4, 18, 30.5 };
    // float sum = 0;
    // for(int i = 0; i < 8; i++){
    //     sum += HI->QRD_Vals[i]*positionVector[i];
    // }
    

    // display.print(sum);
    // display.println(" ");
    // // display.print(float(HI->QRD_Vals[0] - HI->QRD_Min[0])/float(HI->QRD_Max[0] - HI->QRD_Min[0]));
    // // display.print(" ");
    // // display.print(float(HI->QRD_Vals[1] - HI->QRD_Min[1])/float(HI->QRD_Max[1] - HI->QRD_Min[1]));
    // // display.print(" ");
    // // display.print(float(HI->QRD_Vals[2] - HI->QRD_Min[2])/float(HI->QRD_Max[2] - HI->QRD_Min[2]));
    // // display.print(" ");
    // // display.print(float(HI->QRD_Vals[3] - HI->QRD_Min[3])/float(HI->QRD_Max[3] - HI->QRD_Min[3]));
    // // display.print(" ");

    // //HardwareInterface::i()->update();

    // display.display();

    return;
}

