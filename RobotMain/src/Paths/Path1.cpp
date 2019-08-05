
//#include "stateController.h"
 #include "Paths/Path1.h"
 #include "Hardware/HardwareInterface.h"
 #include "Functions/mix.h"
 #include "Functions/drive.h"
 #include "Functions/lineFollow.h"
 #include "Functions/lift.h"

void path1(){
    HardwareInterface* HI = HardwareInterface::i();
    
    //debugging for lift - can delete
    // moveIntake(320, 40, 10000);
    // delay(500);
    // moveIntake(0, 20, 10000);
    // delay(500);
    // moveIntake(320, 40, 10000);
    // delay(500);
    
    // delay(10000000);
    
    
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
            LEdgeTurn();
            break;
        }
    }
    
    //junctino 3
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Post1Turn();
            break;
        }
    }
    //at post now

    //go for stone colleting
    getStoneFromPillar(335,false,10000);

    //on path ready to follow line
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
            QRDTurn(false,900,true,450);
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
    
    delay(1000);
    
    //align 
    turn_single_constant(90,5000,40);
    delay(500);
    turn_single_constant(-90,5000,40);
    delay(500);
    drive_stop_seq(1,2500,25,0,38);
    delay(500);

    //SCORE STONE AND LOWER
    moveIntake(40,18,10000);
    HI->clawMotor->clawSetPos(104);

    jdubDrive(-1, 30, 40, 32, 3000, 0.4, 0.6, 1);

    QRDTurn(false,500,false,0);    

    delay(1000);


    //moving to next pillar:
    //y junctions
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            REdgeTurn();
            break;
        }
    }

    //t_junction for turn
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Post2Turn();
            break;
        }
    }
    //on post now

    //go for stone colleting
    getStoneFromPillar(350, true, 10000);

    //line follower to first Y
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            REdgeTurn();
            break;
        }
    }

    //line follower to nexr y
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            QRDTurn(false,900,true,450);
            break;
        }
    }

    
    startTime = millis();
    //follow line for little more
    while(millis()-startTime < 1500){
        HI->update();
        followTape(35, false, false);
        HI->update();
    }

    //go to detect guanlet
    drive_stop_seq(1,2500,25,0,38);
    
    delay(1000);

    //align 
    turn_single_constant(-90,5000,40);
    delay(500);
    turn_single_constant(90,5000,40);
    delay(500);
    drive_stop_seq(1,2500,25,0,38);
    delay(500);

    
    //SCORE STONE AND LOWER
    moveIntake(40,18,10000);
    HI->clawMotor->clawSetPos(104);


    delay(100000);

}