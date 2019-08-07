#include "Functions/drive.h"
#include "Functions/lineFollow.h"
#include "Functions/lift.h"
#include "Hardware/HardwareInterface.h"
#include "Functions/mix.h"
//#include "stateController.h"
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
                QRDTurn(rightTurnReturn, 500, 42, -33, false, 0); //was 45, -35
        }
        else{
                QRDTurn(rightTurnReturn, 500, -45, 30, false, 0); // was -45, 30
        }
        Serial.println("end turn ");
        delay(150);
}
