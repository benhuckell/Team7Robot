
//#include "stateController.h"
 #include "Paths/Paths.h"
 #include "Hardware/HardwareInterface.h"
 #include "Functions/mix.h"
 #include "Functions/drive.h"
 #include "Functions/lineFollow.h"
 #include "Functions/lift.h"

void path16L(){
    HardwareInterface* HI = HardwareInterface::i();

    // int startLTicks = HI->LEncoder->getCount();
    // int startRTicks = HI->REncoder->getCount();
    // HI->pushDriveSpeeds(60,60/straightLineCorrectionFactor);

    // while(true){
    //     delay(50);
    //     Serial.println("LCount: " + String(HI->LEncoder->getCount()-startLTicks));
    //     Serial.println("RCount: " + String(HI->REncoder->getCount()-startRTicks));
    //     Serial.println("Lspeed: " + String(HI->LEncoder->getSpeed()));
    //     Serial.println("Rspeed: " + String(HI->REncoder->getSpeed()));
    //     Serial.println("");
    //     HI->update();
    // }

    //right  turn : backwards
        //turn_single_constant(-70, 100000, 45);//left turn (works with 28 also, but lag in wheel syart) (no lag with 45)
        //turn_single_constant(-70, 100000, 38);//right turn 


        //HI->pushDriveSpeeds(-48, 30); //turning left -> Quality enough
        HI->pushDriveSpeeds(45, -35); //turning right -> quality enough =
        int aveTicks = 0;
        int target = 1000000000;
        int startingTics = abs(HI->LEncoder->getCount()) + abs(HI->REncoder->getCount()) / 2;
        int currentTics = abs(HI->LEncoder->getCount()) + abs(HI->REncoder->getCount()) / 2;
        while(currentTics - startingTics < target){
             currentTics = abs(HI->LEncoder->getCount()) + abs(HI->REncoder->getCount()) / 2;
             delay(20);
        }

        HI->pushDriveSpeeds(0, 0);


    delay(100000);

drive_stop_seq(1, 800, 10000, 25, 1.4, 50);

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
            Post1Turn(false);
            break;
        }
    }
    //at post now

    //go for stone colleting
    getStoneFromPillar(250,false,10000);

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
            QRDTurn(false,900, 38, true,450);
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
    drive_stop_seq(1,800,2500,25,0,45);

    //line up to drop stone
    
    
    delay(1000);
    
    //align 
    turn_single_constant(90,5000,40);
    delay(500);
    turn_single_constant(-90,5000,40);
    delay(500);
    drive_stop_seq(1,800,2500,25,0,45);
    delay(500);

    //SCORE STONE AND LOWER
    moveIntake(50,18,10000);
    HI->clawMotor->clawSetPos(104);

    jdubDrive(-1, 30, 40, 32, 3000, 0.4, 0.6, 0);

    QRDTurn(false,500, 38, false,0);    

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
            Post6Turn(false);
            break;
        }
    }
    //on post now

    //go for stone colleting
    getStoneFromPillar(367, true, 10000);

    //line follower to first Y
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            LEdgeTurn();
            break;
        }
    }

    //line follower to nexr y
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            QRDTurn(false,900, 38, true,450);
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
    drive_stop_seq(1,800,2500,25,0,45);
    
    delay(1000);

    //align 
    turn_single_constant(-90,5000,40);
    delay(500);
    turn_single_constant(90,5000,40);
    delay(500);
    drive_stop_seq(1,800,2500,25,0,45);
    delay(500);

    
    //SCORE STONE AND LOWER
    moveIntake(40,18,10000);
    HI->clawMotor->clawSetPos(104);


    delay(100000);

}

void path45R(){
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
            REdgeTurn();
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
            REdgeTurn();
            break;
        }
    }
    
    //junctino 3
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Post4Turn(true);
            break;
        }
    }
    //at post now

    //go for stone colleting
    getStoneFromPillar(328,false,10000);

    //on path ready to follow line
    //junction 4
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            LEdgeTurn();
            break;
        }
    }

    //junction 5
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            QRDTurn(true,900, 38, true,450);
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
    drive_stop_seq(1,800,2500,25,0,45);

    //line up to drop stone
    
    
    delay(1000);
    
    //align 
    turn_single_constant(90,5000,40);
    delay(500);
    turn_single_constant(-90,5000,40);
    delay(500);
    drive_stop_seq(1,800,2500,25,0,45);
    delay(500);

    //SCORE STONE AND LOWER
    moveIntake(50,18,10000);
    HI->clawMotor->clawSetPos(104);//open claw

    jdubDrive(-1, 30, 40, 32, 3000, 0.4, 0.6, 0);

    QRDTurn(true,500, 38, false,0);    

    delay(1000);


    //moving to next pillar:
    //y junctions
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            LEdgeTurn();
            break;
        }
    }

    //t_junction for turn
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Post5Turn(true);
            break;
        }
    }
    //on post now

    //go for stone colleting
    getStoneFromPillar(357, true, 10000);

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
            QRDTurn(true,900, 38, true,450);
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
    drive_stop_seq(1,800,2500,25,0,45);
    
    delay(1000);

    //align 
    turn_single_constant(-90,5000,40);
    delay(500);
    turn_single_constant(90,5000,40);
    delay(500);
    drive_stop_seq(1,800,2500,25,0,45);
    delay(500);

    
    //SCORE STONE AND LOWER
    moveIntake(40,18,10000);
    HI->clawMotor->clawSetPos(104);


    delay(100000);

}

void path21L(){
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
            REdgeTurn();
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
            REdgeTurn();
            break;
        }
    }
    
    //junctino 3
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Post4Turn(false);
            break;
        }
    }
    //at post now

    //go for stone colleting
    getStoneFromPillar(328,false,10000);

    //on path ready to follow line
    //junction 4
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            LEdgeTurn();
            break;
        }
    }

    //junction 5
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            QRDTurn(true,900, 38, true,450);
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
    drive_stop_seq(1,800,2500,25,0,45);

    //line up to drop stone
    
    
    delay(1000);
    
    //align 
    turn_single_constant(90,5000,40);
    delay(500);
    turn_single_constant(-90,5000,40);
    delay(500);
    drive_stop_seq(1,800,2500,25,0,45);
    delay(500);

    //SCORE STONE AND LOWER
    moveIntake(50,18,10000);
    HI->clawMotor->clawSetPos(104);//open claw

    jdubDrive(-1, 30, 40, 32, 3000, 0.4, 0.6, 0);

    QRDTurn(true,500, 38, false,0);    

    delay(1000);


    //moving to next pillar:
    //y junctions
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            LEdgeTurn();
            break;
        }
    }

    //t_junction for turn
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Post5Turn(false);
            break;
        }
    }
    //on post now

    //go for stone colleting
    getStoneFromPillar(357, true, 10000);

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
            QRDTurn(true,900, 38, true,450);
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
    drive_stop_seq(1,2500,400,25,0,45);
    
    delay(1000);

    //align 
    turn_single_constant(-90,5000,40);
    delay(500);
    turn_single_constant(90,5000,40);
    delay(500);
    drive_stop_seq(1,2500,400,25,0,45);
    delay(500);

    
    //SCORE STONE AND LOWER
    moveIntake(40,18,10000);
    HI->clawMotor->clawSetPos(104);


    delay(100000);

}