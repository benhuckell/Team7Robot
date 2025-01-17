#include "Paths/Path561Left.h"
#include "Hardware/HardwareInterface.h"
#include "Functions/mix.h"
#include "Functions/drive.h"
#include "Functions/lineFollow.h"
#include "Functions/lift.h"

void path561L(){




    HardwareInterface* HI = HardwareInterface::i();
    
    int robotSpeed = 50;
    int startTime = millis();

    //Start Following Tape until first Junction
    while(true){
        HI->update();
        if (millis() - startTime < 7000){
            followTape(60, false, false);}
        else{
            followTape(40, false, false); 
        }
        if (detectBumpInRoad(0.35)){
            //do nothing
        }
        else if(detectJunction(0.85)){
            LEdgeTurn();
            break;
        }
    }
    
    //START OF STONE 1 SEQUENCE//
    //J1 to J2
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
    
    //J2 to Post1
    while(true){
        HI->update();
        followTape(40, false, false);
        if(detectJunction()){
            Post5Turn(false);
            break;
        }
    }
    
    //at post now, lined up

    //Collect Stone from Post 1
    //getStoneTopDown(345, 342, 357, true, 10000);
    getStoneMAXUPReset(1200, 348, 380, true, 10000);
    
    //On path ready to follow line back to gauntlet
    //junction 4 -Y - Stone1 to gauntlet
    while(true){
        HI->update();
        followTape(40, false, false);
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

    //junction 5 - Gaunt - stone1 to gaunt
    while(true){
        HI->update();
        followTape(40, false, false);
        if(detectJunction()){
            //Follow Tape, then turn to gauntlet
            QRDTurn(false,900, -33, 30, true,500);
            break;
        }
    }

    
    startTime = millis();
    //follow line for little more
    while(millis()-startTime < 1300){
        HI->update();
        followTape(35, false, false);
    }

    //Drive until hit gauntlet
    drive_stop(45, 45/1.13, 70, 70/1.13, 300, 1500, 50, -33);

    //line up to drop stone
    jiggle();
    delay(300);

    //Drop Stone 1
    //Turn to gauntlet hole angle
    //turn_single_constant(-29, 10000, 45);
    time_turn(true, 46, 370, 10);
    //Begin to move intake down
    moveIntake(45,18,5000);
    delay(500);

    //Jolt Forwareds
    HI->pushDriveSpeeds(45, 45/1.13);
    delay(130);

    //Begin opening claw
    HI->clawMotor->clawSetPos(30);

    //Crawl to hole
    HI->pushDriveSpeeds(32, 32/1.13);
    delay(200);

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
    QRDTurn(false, 200, -32, 32, false,0);   // more right, less left 
    delay(500);


    //START OF STONE 2 SEQUENCE//
    //J1 to J2
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            REdgeTurn();
            break;
        }
    }

    //t_junction for turn - getting stone 2
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Post6Turn(false);
            break;
        }
    }
    //at post, lined up

    //Collect Stone 2
    //getStoneTopDown(345, 342, 357, true, 10000);
    getStoneMAXUPReset(1600, 348, 380, true, 10000);
    
    //On path ready to follow line back to gauntlet
    //line follower to  Y - stone 2 to gaunt
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
            QRDTurn(false,900, -33, 30, true,500);
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

    //Drive until hit gauntlet
    drive_stop(45, 45/1.13, 70, 70/1.13, 200, 1500, 50, -33);

    //line up to drop stone
    jiggle();
    delay(800);


    //Turn to line up with hole
    //turn_single_constant(28, 100000, 36);
    time_turn(false, 36, 442, 10);
    delay(500);
    //Begin to lower intake
    moveIntake(42,18,10000);
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

    //Stone 2 Scored!


    //Get back to line
    HI->pushDriveSpeeds(-58, -60/1.13);//slowed down left time
    delay(450);
    HI->pushDriveSpeeds(0, 0);
    delay(300);
    QRDTurn(false, 300, -32, 32, false,0);    
    delay(200);


    //START OF STONE 3 SEQUENCE//
    //Y junctions - getting stone 3
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            LEdgeTurn();
            break;
        }
    }


    //Line up with third post
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            Post1Turn(false);
            break;
        }
    }

    //Go back a bit more
    HI->pushDriveSpeeds(-33,-33/1.13);
    delay(105);
    HI->pushDriveSpeeds(0,0);

    //At post, lined up

    //Retrieve stone from post
    //getStoneTopDown(265, 242, 290, false, 10000);
    getStoneMAXUPReset(1300, 237, 290, false, 10000);
    //Collected stone, on line

    //line follower to  Y - stone 3 to gaunt
    while(true){
        HI->update();
        followTape(robotSpeed, false, false);
        if(detectJunction()){
            REdgeTurn();
            break;
        }
    }

    //line follower to Gaunt. stone 3 to gaunt
    while(true){
        HI->update();
        followTape(40, false, false);
        if(detectJunction()){
            QRDTurn(false,900, -33, 30, true,500);
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
    delay(500);

    //Turn to line up with hole
    //turn_single_constant(-17, 100000, 45);
    time_turn(true, 45, 330, 10);
    delay(500);
//
    HI->pushDriveSpeeds(-33,-33/1.13);
    delay(100);
    HI->pushDriveSpeeds(0,0);
    //Begin to lower intake
    moveIntake(46,18,10000);
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

    delay(100000);

    //Stone 3 Scored!
}