
 #include "Functions/drive.h"
 #include "Functions/lineFollow.h"
 #include "Functions/lift.h"
 #include "Hardware/HardwareInterface.h"
 #include "Functions/mix.h"
//#include "stateController.h"
void getStoneFromPillar(int tickTargetWinch, bool rightTurnReturn, int turnTimeout){
        HardwareInterface* HI = HardwareInterface::i(); 
        Serial.println("Stone Collecting");
        getStone(tickTargetWinch);

        delay(100);
        Serial.println("start backing out");
        jdubDrive(-1, 35, 40, 32, 3000, 0.4, 0.6, 1);
        delay(1500);
        Serial.println("QRD turn Hardware init");
        QRDTurn(rightTurnReturn, 500,false,0);
        Serial.println("end turn ");
        delay(1500);
}
