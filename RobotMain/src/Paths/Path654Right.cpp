#include "Paths/Path654Right.h"
#include "Hardware/HardwareInterface.h"
#include "Functions/mix.h"
#include "Functions/drive.h"
#include "Functions/lineFollow.h"
#include "Functions/lift.h"

void path654R(){


    HardwareInterface* HI = HardwareInterface::i();
    
    int robotSpeed = 50;
    int robotSpeed_slow = 50;
    int startTime = millis();

    //Start Following Tape until first Junction
    while(true){
        HI->update();
        if (millis() - startTime < 7000){
            followTape(60, false, false);}
        else{
            followTape(38, false, false); 
        }
        if (detectBumpInRoad(0.3)){
            //do nothing
        }
        else if(detectJunction(0.85)){
            REdgeTurn();
            //REdgeTurn_CUSTOM();
            break;
        }
    }
    
    //START OF STONE 1 SEQUENCE//
    //J1 to J2
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            LEdgeTurn();
            break;
        }
    }

    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            REdgeTurn();
            break;
        }
    }
    
    //J2 to Post1
    while(true){
        HI->update();
        followTape(40, false, false);
        if(detectJunction()){
            Post6Turn(true);
            break;
        }
    }
    
    //at post now, lined up

    //Collect Stone from Post 1
    //getStoneFromPillar(330, 357, false, 10000); 
    getStoneMAXUPReset(1000, 348, 380, false, 10000);
    //On path ready to follow line back to gauntlet
    //junction 4 -Y - Stone1 to gauntlet
    while(true){
        HI->update();
        followTape(40, false, false);
        if(detectJunction()){
            LEdgeTurn();
            break;
        }
    }

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
            //Follow Tape, then turn to gauntlet
            QRDTurn(true,900, 33, -30, true,500);
            break;
        }
    }

    
    startTime = millis();
    //follow line for little more
    while(millis()-startTime < 1300){
        HI->update();
        followTape(39, false, false);
    }

    //Drive until hit gauntlet
    drive_stop(45, 45/1.13, 70, 70/1.13, 300, 1500, 50, -33);

    //line up to drop stone
    jiggle();
    delay(300);

    //Drop Stone 1
    //Turn to gauntlet hole angle
    //turn_single_constant(29, 5000, 36);
    time_turn(false, 36, 440, 10);
    //Begin to move intake down
    moveIntake(43,18,10000);
    delay(500);

    //Jolt Forwareds
    HI->pushDriveSpeeds(45, 45/1.13);
    delay(130);

    //Begin opening claw
    HI->clawMotor->clawSetPos(30);

    //Crawl to hole
    HI->pushDriveSpeeds(32, 32/1.13);
    delay(150); //was 300 NEW

    //Stop moving once stone deposited
    HI->pushDriveSpeeds(0, 0);
    delay(800);

    //Tighten slack
    HI->pushWinchSpeed(25);
    delay(900);

    //Stone Scored

    //Return to Line
    HI->pushDriveSpeeds(-60, -60/1.13);
    delay(250);
    HI->pushDriveSpeeds(0, 0);
    QRDTurn(true, 300, 33, -29, false,0);   // more right, less left 
    delay(500);


    //START OF STONE 2 SEQUENCE//
    //J1 to J2
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
            Post5Turn(true);
            break;
        }
    }
    //at post, lined up

    //Collect Stone 2
    //getStoneFromPillar(330, 357, false, 10000); 
     getStoneMAXUPReset(1600, 348, 380, false, 10000);

    //On path ready to follow line back to gauntlet
    //line follower to  Y - stone 2 to gaunt
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            REdgeTurn();
            break;
        }
    }

    //line follower to Gaunt. stone 2 to gaunt
    while(true){
        HI->update();
        followTape(40, false, false);
        if(detectJunction()){
            QRDTurn(true,700, 33, -30, true,500);
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

    //Drive until hit gauntlet
    drive_stop(45, 45/1.13, 70, 70/1.13, 200, 1500, 50, -33);

    //line up to drop stone
    jiggle();
    delay(800);

    //Turn to line up with hole
    //turn_single_constant(-28, 5000, 45);
    time_turn(true, 46, 430, 10);
    delay(500);
    //Begin to lower intake
    moveIntake(35,18,10000);
    delay(500);

    //Jolt forwards
    HI->pushDriveSpeeds(45, 45/1.13);
    delay(130);

    //Begin to lower claw
    HI->clawMotor->clawSetPos(30);

    //Crawl forwards
    HI->pushDriveSpeeds(32, 32/1.13);
    delay(800);

    //Stop moving
    HI->pushDriveSpeeds(0, 0);
    delay(800);

    //Tighten slack
    HI->pushWinchSpeed(25);
    delay(900);

    //Stone 2 SCORED!


    //Get back to line
    HI->pushDriveSpeeds(-58, -60/1.13);//slowed down left time
    delay(450);
    HI->pushDriveSpeeds(0, 0);
    delay(300);
    QRDTurn(true, 375, 33, -29, false,0);    
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

    //Line up with third post
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Post4Turn(true);
            break;
        }
    }

    //Go back a bit more
    HI->pushDriveSpeeds(-33,-33/1.13);
    delay(130);
    HI->pushDriveSpeeds(0,0);
    
    //At post, lined up

    //Retrieve stone from post
    //getStoneFromPillar(250, 285, true,10000); //was 225 //228, just barely low
    getStoneMAXUPReset(1200, 237, 290, true, 10000);
    //Collected stone, on line

    //line follower to  Y - stone 3 to gaunt
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
            QRDTurn(true,900, 33, -30, true,500);
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

    //Drive until hit gauntlet
    drive_stop(45, 45/1.13, 70, 70/1.13, 300, 1500, 50, -33);

    //line up to drop stone 3
    jiggle();
    delay(300);

    //Turn to line up with hole
    //turn_single_constant(-17, 100000, 45);
    time_turn(true, 45, 355, 10);

//
    //Begin to lower intake
    moveIntake(47,18,10000);
    delay(500);

    //Jolt forwards
    HI->pushDriveSpeeds(55, 55/1.13);
    delay(130);

    //Begin to lower claw
    HI->clawMotor->clawSetPos(30);

    //Crawl forwards
    HI->pushDriveSpeeds(37, 37/1.13);
    delay(800);

    //Stop moving
    HI->pushDriveSpeeds(0, 0);
    delay(800);
    //

    delay(100000);

    //Stone 3 Scored!
}