
//#include "stateController.h"
 #include "Paths/Path1.h"
 #include "Hardware/HardwareInterface.h"
 #include "Functions/mix.h"
 #include "Functions/drive.h"
 #include "Functions/lineFollow.h"
 #include "Functions/lift.h"

void path1(){
    HardwareInterface* HI = HardwareInterface::i();
    int robotSpeed = 50;
    //junction1
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Serial.println("J1 Detected!");
            Serial.println("-------------------------------------");
            LEdgeTurn();
            break;
        }

    }
    //junction 2
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Serial.println("J2 Detected!");
            Serial.println("-------------------------------------");
            delay(10000000);//for debug
            LEdgeTurn();
            break;
        }
    }
    
    //junctino 3
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            PostTurnLeft();
            break;
        }
    }


    //go for stone colleting
    getStoneFromPillar1();

    //junction 4
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            REdgeTurn();
            break;
        }
    }

    //junction 5
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            QRD_Left();
            break;
        }
    }

    
    int startTime = millis();
    //follow line for little more
    while(millis()-startTime < 1500){
        HI->update();
        followTape(35, false, false);
        HI->update();
    }

    //go to detect guanlet
    drive_stop_seq(1,2500,25,0,38);
    delay(100000);

}