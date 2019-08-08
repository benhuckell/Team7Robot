#include "Functions/drive.h"
#include "Functions/lineFollow.h"
#include "Functions/lift.h"
#include "Hardware/HardwareInterface.h"
#include "Functions/mix.h"
//#include "stateController.h"
extern int currentHeight;

void getStoneFromPillar(int tickTargetWinch, int tickTargetWinch_2, bool rightTurnReturn, int turnTimeout){
        HardwareInterface* HI = HardwareInterface::i(); 
        Serial.println("Stone Collecting");

        getStone(tickTargetWinch, tickTargetWinch_2);

        delay(100);
        Serial.println("start backing out");
        //jdubDrive(-1, 35, 40, 32, 3000, 0.4, 0.6, 0);
           HI->pushDriveSpeeds(-60, -60/1.13);
        delay(230);
        HI->pushDriveSpeeds(0, 0);
        delay(200);
        Serial.println("QRD turn Hardware init");
        
        if(rightTurnReturn){
                QRDTurn(rightTurnReturn, 500, 33, -30, false, 0); //was 45, -35
        }
        else{
                QRDTurn(rightTurnReturn, 500, -33, 30, false, 0); // was -45, 30
        }
        Serial.println("end turn ");
        delay(150);
}

void getStoneTopDown(int tickTargetWinch, int tickTargetWinch_2, int tickTargetWinch_3, bool rightTurnReturn, int turnTimeout){
        HardwareInterface* HI = HardwareInterface::i(); 
        Serial.println("Stone Collecting");
       
        //getStone(tickTargetWinch, tickTargetWinch_2);
        
            //Move claw to stone height
        moveIntake(tickTargetWinch,70,5000);
        delay(200);

        //Close claw a little
        HI->clawMotor->clawSetPos(180);
        delay(600);

        //Move claw to stone height
        moveIntake(tickTargetWinch_2,15,5000);
        delay(200);

        //grab
        HI->clawMotor->clawSetPos(210);

         HI->pushWinchSpeed(25);
        delay(600);

        //Move claw up to clear stone
        moveIntake(tickTargetWinch_3,70,3000);

        delay(30);
        Serial.println("start backing out");
        //jdubDrive(-1, 35, 40, 32, 3000, 0.4, 0.6, 0);
           HI->pushDriveSpeeds(-60, -60/1.13);
        delay(230);
        HI->pushDriveSpeeds(0, 0);
        delay(200);
        Serial.println("QRD turn Hardware init");
        
        if(rightTurnReturn){
                QRDTurn(rightTurnReturn, 500, 34, -30, false, 0); //was 45, -35
        }
        else{
                QRDTurn(rightTurnReturn, 500, -34, 30, false, 0); // was -45, 30
        }
        Serial.println("end turn ");
        delay(150);
}


void getStoneMAXUPReset(int timeup, int resetTarget, int winchTargetUp, bool rightTurnReturn, int turnTimeout){
        HardwareInterface* HI = HardwareInterface::i(); 
        Serial.println("Stone Collecting");
       

              ///NEW INTAKE WINCH
       
       HI->pushWinchSpeed(70);
       delay(timeup);
       HI->pushWinchSpeed(0);
       HI->clawMotor->clawSetPos(182);
       delay(400);
       HI->pushWinchSpeed(-15);
       delay(700);
        HI->pushWinchSpeed(25);
        HI->clawMotor->clawSetPos(210);
        delay(1200);
        HI->pushWinchSpeed(0);
        currentHeight = resetTarget;
        delay(100);

        moveIntake(winchTargetUp,55,5000);
        delay(30);
        Serial.println("start backing out");
        //jdubDrive(-1, 35, 40, 32, 3000, 0.4, 0.6, 0);
           HI->pushDriveSpeeds(-60, -60/1.13);
        delay(230);
        HI->pushDriveSpeeds(0, 0);
        delay(200);
        Serial.println("QRD turn Hardware init");
        
        if(rightTurnReturn){
                QRDTurn(rightTurnReturn, 500, 34, -30, false, 0); //was 45, -35
        }
        else{
                QRDTurn(rightTurnReturn, 500, -34, 30, false, 0); // was -45, 30
        }
        Serial.println("end turn ");
        delay(150);
}
