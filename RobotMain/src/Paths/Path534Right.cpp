#include "Paths/Path534Right.h"
#include "Hardware/HardwareInterface.h"
#include "Functions/mix.h"
#include "Functions/drive.h"
#include "Functions/lineFollow.h"
#include "Functions/lift.h"

void path534R(){
    HardwareInterface* HI = HardwareInterface::i();
    
    int robotSpeed = 50;
    int startTime = millis();

    //Start following tape up ramp
    while(true){
        HI->update();
        if (millis() - startTime < 7000){
            followTape(60, false, false);}
        else{
           followTape(50, false, false); 
        }
        if (detectBumpInRoad(0.35)){
            //do nothing
        }
        else if(detectJunction(0.85)){
            REdgeTurn();
            break;
        }

    }

    //START OF STONE 1 SEQUENCE//
    //junction 2 - Y - Getting stone 1
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
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

    //at post, lined up

    //go for stone colleting first post
    //getStoneFromPillar(328, 350, false, 10000);
    //new
    getStoneTopDown(372, 348, 372, false, 10000);

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
        followTape(40, false, false);
        if(detectJunction()){
            QRDTurn(true,730, 33, -30, true,500);
            break;
        }
    }

    //follow line for little more
    startTime = millis();
    while(millis()-startTime < 1300){
        HI->update();
        followTape(35, false, false);
    }

    //go to detect guanlet
    drive_stop(45, 45/1.13, 70, 70/1.13, 300, 1500, 50, -33);

    //line up to drop stone
    jiggle();
    delay(300);

    //Line up with gauntlet hole
    turn_single_constant(27, 100000, 38);

    //Lower intake 
    moveIntake(37,18,5000);
    delay(500);

    //jolt forwards
    HI->pushDriveSpeeds(45, 45/1.13);
    delay(130);

    //open claw to release stone
    HI->clawMotor->clawSetPos(30);

     //push stone into hole
    HI->pushDriveSpeeds(32, 32/1.13);
    delay(800);

    //first STOone Scored!
    //SCORE STONE AND LOWER
    HI->pushDriveSpeeds(0, 0);
    delay(800);

    //Tighten slack
    HI->pushWinchSpeed(25);
    delay(900);
    
    //back up and return to tape
    HI->pushDriveSpeeds(-60, -60/1.13);
    delay(250);
    HI->pushDriveSpeeds(0, 0);
    delay(300);
    QRDTurn(true, 200, 33, -29, false,0);   // more right, less left 

    //Y junctions - GETTING STONE #2
    //J1
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
    //getStoneFromPillar(156, 200, true, 10000);  
    getStoneTopDown(170, 145, 200, true, 10000);

    // //line follower to  Y - stone 2 to gaunt
    // startTime = millis();
    //  while(millis()-startTime < 400){
    //     HI->update();
    //     followTape(robotSpeed, false, false);
    // }


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
        followTape(40, false, false);
        if(detectJunction()){
            QRDTurn(true,730, 33, -30, true,500);
            break;
        }
    }

    startTime = millis();
    //follow line for little more
    while(millis()-startTime < 1300){
        HI->update();
        followTape(35, false, false);
        HI->update();
    }

    //go to detect guanlet
    drive_stop(45, 45/1.13, 70, 70/1.13, 200, 1500, 50, -33);

    //line up to drop stone
    jiggle();
    delay(800);
    
    //Line up with gauntlet hole
    turn_single_constant(-27, 100000, 45);//left

    //Lower intake
    moveIntake(35,18,10000);//
    delay(500);

    //Jolt forwards
    HI->pushDriveSpeeds(45, 45/1.13);
    delay(130);
   
    //Open Claw
    HI->clawMotor->clawSetPos(30);

    //Crawl Forwards
    HI->pushDriveSpeeds(32, 32/1.13);
    delay(800);

    //Stop Moving
    HI->pushDriveSpeeds(0, 0);
    delay(800); 

    //Tighten slack
    HI->pushWinchSpeed(25);
    delay(900);    

    //Stone 2 Scored!

    //Backup and return to line
    HI->pushDriveSpeeds(-58, -60/1.13);
    delay(450);
    HI->pushDriveSpeeds(0, 0);
    delay(300);
    QRDTurn(true, 300, 33, -29, false,0);    
    delay(200);


    //START OF STONE 3 SEQUENCE//
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

    //Go back a bit more
    HI->pushDriveSpeeds(-33,-33/1.13);
    delay(105);
    HI->pushDriveSpeeds(0,0);

    getStoneTopDown(265, 237, 290, true, 10000);

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
        followTape(40, false, false);
        if(detectJunction()){
            QRDTurn(true,730, 33, -30, true,500);
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
    drive_stop(45, 45/1.13, 70, 70/1.13, 300, 1500, 50, -33);

    //line up to drop stone 3
    jiggle();
    delay(800);
    
    //Line up with gauntlet hole
    turn_single_constant(-17, 100000, 45);//left
    delay(500);

    HI->pushDriveSpeeds(-33,-33/1.13);
    delay(100);
    HI->pushDriveSpeeds(0,0);

    //Lower intake
    moveIntake(43,18,10000);//
    delay(500);

    //Jolt forwards
    HI->pushDriveSpeeds(65, 65/1.13);
    delay(150);
   
    //Open Claw
    HI->clawMotor->clawSetPos(30);

    //Crawl Forwards
    HI->pushDriveSpeeds(37, 37/1.13);
    delay(800);

    //Stop Moving
    HI->pushDriveSpeeds(0, 0);

    delay(100000);

    //Stone 2 Scored!

}

