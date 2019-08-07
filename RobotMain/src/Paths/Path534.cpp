
//#include "stateController.h"
 #include "Paths/Paths.h"
 #include "Hardware/HardwareInterface.h"
 #include "Functions/mix.h"
 #include "Functions/drive.h"
 #include "Functions/lineFollow.h"
 #include "Functions/lift.h"
 #include "Paths/Path534.h"

void path534_R(){
    HardwareInterface* HI = HardwareInterface::i();
    
    int robotSpeed = 50;
    //junction1 - Gaunt - Getting stone 1


    int startTime = millis();

    while(true){
        HI->update();
        if (millis() - startTime < 7350){
            followTape(60, false, false);}
        else{
           followTape(45, false, false); 
        }
        if(detectJunction()){
            Serial.println("J1 Detected!");
            Serial.println("-------------------------------------");
            REdgeTurn();
            break;
        }

    }
    //junction 2 - Y - Getting stone 1
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
    
    //junctino 3 - T1 Getting stone 1
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Post5Turn(true);
            break;
        }
    }
    //at post now

    //go for stone colleting first post
    getStoneFromPillar(330, 370, false, 10000);

    //on path ready to follow line
    //junction 4 -Y - Stone1 to gaunt
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            REdgeTurn();
            break;
        }
    }

    //junction 5 - Gaunt - stone1 to gaunt
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            QRDTurn(true,900, 39, -35, true,600);
            break;
        }
    }

    
    startTime = millis();
    //follow line for little more
    while(millis()-startTime < 1300){
        HI->update();
        followTape(39, false, false);
    }

    //go to detect guanlet
     drive_stop(45, 45/1.13, 70, 70/1.13, 300, 1500, 50);

    //line up to drop stone
    jiggle();
    delay(300);

    
    //old double turn
    // //left
    // turn_single_constant(101, 100000, 38); //right // was 107 // 105 (too muach) //103 toomuch 
    // delay(750);

    // turn_single_constant(-106, 100000, 45);//left // was -103 //waas -104 U //105 U
    // delay(750);
    // drive_stop(45, 45/1.13, 70, 70/1.13, 200, 1500, 50);


        // //SCORE FIRST STONE
    turn_single_constant(32, 100000, 36); //right // was 107 // 105 (too muach) //103 toomuch 
    moveIntake(42,18,10000);
    delay(500);
    HI->pushDriveSpeeds(50, 50/1.13);
    delay(130);

     HI->clawMotor->clawSetPos(30);
     delay(40);
    HI->pushDriveSpeeds(33, 33/1.13);
    delay(800);

    //first STOone ScoredQ!
    //SCORE STONE AND LOWER
    HI->pushDriveSpeeds(0, 0);
    delay(800);
    //jdubDrive(-1, 30, 40, 32, 3000, 0.4, 0.6, 0);
    HI->pushDriveSpeeds(-60, -60/1.13);
    delay(140);
    HI->pushDriveSpeeds(0, 0);


    QRDTurn(true, 300, 39, -35, false,0);   // more right, less left 

    delay(500);


    //moving to next pillar:
    //Y junctions - getting stone 2
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            REdgeTurn();
            break;
        }
    }


    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            LEdgeTurn();
            break;
        }
    }

    //t_junction for turn - getting stone 2
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Post3Turn(true);
            break;
        }
    }
    //on post now

    //go for stone colleting second post
    getStoneFromPillar(168, 245, true, 10000);  // 165, too high

    //line follower to  Y - stone 2 to gaunt
    startTime = millis();
     while(millis()-startTime < 400){
        HI->update();
        followTape(robotSpeed, false, false);
    }


    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            REdgeTurn();
            break;
        }
    }

    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            LEdgeTurn();
            break;
        }
    }

    //line follower to Gaunt. stone 2 to gaunt
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            QRDTurn(true,900, 39, -35, true,600);
            break;
        }
    }

    
    startTime = millis();
    //follow line for little more
    while(millis()-startTime < 1300){
        HI->update();
        followTape(39, false, false);
        HI->update();
    }

       //go to detect guanlet
     drive_stop(45, 45/1.13, 70, 70/1.13, 200, 1500, 50);

    //line up to drop stone
    
    jiggle();
    delay(800);
    

    // turn_single_constant(-108, 100000, 45);//left
    // delay(750);
    // turn_single_constant(107, 100000, 38); //right
    // delay(750);
    // drive_stop(45, 45/1.13, 70, 70/1.13, 300, 1500, 50);
    //  SCORE SECOND STONE

    turn_single_constant(-31, 100000, 45);//left
    delay(500);
    moveIntake(42,18,10000);//
    delay(550);
    HI->pushDriveSpeeds(50, 50/1.13);
    delay(130);
   
    HI->clawMotor->clawSetPos(30);
    delay(40);
    HI->pushDriveSpeeds(33, 33/1.13);
    delay(800);
    HI->pushDriveSpeeds(0, 0);
    delay(800);
    // turn_single_constant(107, 100000, 38); //right
    // delay(750);
    // drive_stop(45, 45/1.13, 70, 70/1.13, 300, 1500, 50);
    //       


    //Stone 2 Scored!

    HI->pushDriveSpeeds(-58, -60/1.13);//slowed down left time
    delay(450);//550;
    HI->pushDriveSpeeds(0, 0);
    delay(300);

    QRDTurn(true, 300, 39, -35, false,0);    

    delay(200);


    //moving to next pillar:
    //Y junctions - getting stone 3
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            REdgeTurn();
            break;
        }
    }


    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            //moveIntake(250, 48, 2500);
            Post4Turn(true);
            break;
        }
    }
    getStoneFromPillar(315, 350, true,10000); //was 225 //228, just barely low

    //T - stone 3 to gaunt
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            LEdgeTurn();
            break;
        }
    }

    //line follower to Gaunt. stone 3 to gaunt
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            QRDTurn(true,900, 39, -35, true,600);
            break;
        }
    }

    
    startTime = millis();
    //follow line for little more
    while(millis()-startTime < 1300){
        HI->update();
        followTape(39, false, false);
        HI->update();
    }

       //go to detect guanlet
     drive_stop(45, 45/1.13, 70, 70/1.13, 300, 1500, 50);

    //line up to drop stone 3
     jiggle();
    
    delay(1000);
    

    
    turn_single_constant(-24, 100000, 45);//left
    delay(500);
    

    HI->pushDriveSpeeds(60, 60/1.13);
    delay(350);
    HI->pushDriveSpeeds(0, 0);
    delay(100);


    //     //align 
    // turn_single_constant(-108, 100000, 45);//left
    // delay(750);
    // turn_single_constant(107, 100000, 38); //right
    // delay(750);
    // drive_stop(49, 50/1.15, 70, 70/1.15, 300, 1500, 50);

        //SCORE Thrid Stone
        //SCORE STONE AND LOWER
    moveIntake(42,18,10000);
    //HI->clawMotor->clawSetPos(10);
    
    HI->pushDriveSpeeds(38, 38/1.13);
    delay(300);
    HI->pushDriveSpeeds(0, 0);
    delay(800);

    delay(100000);

    //Stone 2 Scored!

}

