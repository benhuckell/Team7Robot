//#include <Wire.h>
#include <queue>
#include <Adafruit_SSD1306.h>
#include "Arduino.h"
#include "StateLoops/lineFollow.h"
#include "configGlobals.h"

#define OLED_RESET -1  // Not used

Adafruit_SSD1306 display(OLED_RESET);

#define PWM_LMOTOR_FORWARDS PA_0 //motor ports
#define PWM_LMOTOR_BACKWARDS PA_1
#define PWM_RMOTOR_FORWARDS PA_2
#define PWM_RMOTOR_BACKWARDS PA_3

#define QRD_IN1 PA_4 //QRD read port
#define QRD_IN2 PA_5 //QRD read port

#define Select_0 PA_15 //mux selects
#define Select_1 PB_3
#define Select_2 PB_4
 
namespace StateLoops {
    //enum lineFollowStates{online,offline} lineFollowState;
    
    LineFollow(QRD QRD_Array[QRD::NUM_QRD_SENSORS]){
        
        float QRD_Calib_Max[QRD::NUM_QRD_SENSORS];
        float QRD_Calib_Min[QRD::NUM_QRD_SENSORS];
        float EDGE_QRD_VAL[QRD::NUM_QRD_SENSORS];

        //initialize QRD reference values
        QRD_Calib_Max[0] = 250;
        QRD_Calib_Max[1] = 250;
        QRD_Calib_Max[2] = 250;
        QRD_Calib_Max[3] = 250;

        QRD_Calib_Min[0] = 0;
        QRD_Calib_Min[1] = 0;
        QRD_Calib_Min[2] = 0;
        QRD_Calib_Min[3] = 0;
 
        Hardware::QRD qrd0;
        Hardware::QRD qrd1;
        Hardware::QRD qrd2;
        Hardware::QRD qrd3;
        Hardware::QRD qrdLeft;
        Hardware::QRD qrdRight;

        QRD_Array[0] = qrd0;
        QRD_Array[1] = qrd1;
        QRD_Array[2] = qrd2;
        QRD_Array[3] = qrd3;
    }

    /**
     * The line follow state loop that calls other functions based on different conditions
     */
    void LineFollow::loop() {
        if(LineFollow::postDetected){
            findPost();
        }
        else if(LineFollow::lostLine){
            findLine();
        }
        else{
            followTape();
        }
    }

    /**
     * Returns the current amount of line following error
     */
    float LineFollow::getLinePositionError()
    {
        // Read from QRD sensors
        float QRD_Out[QRD::NUM_QRD_SENSORS];

        for(int n = 0; n < QRD::NUM_QRD_SENSORS; n++)
        {
            // Update Select output ports to increment upwards in binary with 'n' -> 000, 001, 010, etc
            digitalWrite(Select_0, n&1);
            digitalWrite(Select_1, (n&2)!=0);
            digitalWrite(Select_2, (n&4)!=0);

            QRD_Out[n] = analogRead(QRD_IN1); //read from first multiplexer
            QRD_Out[QRD::NUM_QRD_SENSORS/2 + n] = analogRead(QRD_IN2); //read from second multiplexer
        }

        //Find largest and smallest sensor readings
        for(int i = 0; i < QRD::NUM_QRD_SENSORS; i++){
            QRD_Calib_Max[i] = QRD_Out[0];
            QRD_Calib_Min[i] = QRD_Out[0];
        }
        for(int i = 1; i < QRD::NUM_QRD_SENSORS; i++)
        {
            if(QRD_Out[i] > QRD_Calib_Max[i]){
                QRD_Calib_Max[i] = QRD_Out[i];
            }
            else if(QRD_Out[i] < QRD_Calib_Min[i]){
                QRD_Calib_Min[i] = QRD_Out[i];
            }
            EDGE_QRD_VAL[i] = (QRD_Calib_Max[i] + QRD_Calib_Min[i])*0.5;
        }

        //Calculate QRD value for edge
        float target;
        float leftEdgeXVal = 0;
        float rightEdgeXVal = 0;
        int i = 0;
        float edgeXPos = 0;
        if(LineFollow::followRightEdge){ 
            //Find right edge
            for(i = QRD::NUM_QRD_SENSORS-1; ((QRD_Out[i]-QRD_Calib_Min[i]) > EDGE_QRD_VAL[i]-QRD_Calib_Min[i]) && (i >= 0); i--)
            ;//intentionally blank for loop
            if(i == QRD::NUM_QRD_SENSORS-1){ rightEdgeXVal = 0 + (QRD::NUM_QRD_SENSORS-1)*0.5; } //handle case where line is directly below rightmost sensor
            else{
                //interpolate and subtract (NUM_QRD_SENSORS-1)/2 to put a zero x value in the middle of the sensor array
                rightEdgeXVal = i + ( (EDGE_QRD_VAL[i]-QRD_Calib_Min[i]) - (QRD_Out[i]-QRD_Calib_Min[i]) )/( (QRD_Out[i+1]-QRD_Calib_Min[i]) - (QRD_Out[i]-QRD_Calib_Min[i]) ) - (NUM_QRD_SENSORS-1)*0.5;
            }
            edgeXPos = rightEdgeXVal;
            target = 1;
        }
        else{
            //Find left edge
            int i;
            for(i = 0; (QRD_Out[i] > EDGE_QRD_VAL[i]) && (i < QRD::NUM_QRD_SENSORS); i++)
            ;//intentionally empty for loop
            if(i == 0){ leftEdgeXVal = 0 - (QRD::NUM_QRD_SENSORS-1)*0.5; } //handle case where line is directly below leftmost sensor
            else{
                //interpolate and subtract (NUM_QRD_SENSORS-1)/2 to put a zero x value in the middle of the sensor array
                leftEdgeXVal = i - (EDGE_QRD_VAL[i] - QRD_Out[i])/(QRD_Out[i-1] - QRD_Out[i]) - (QRD::NUM_QRD_SENSORS-1)*0.5;
            }
            edgeXPos = leftEdgeXVal;
            target = -1;
        }
        if((rightEdgeXVal - leftEdgeXVal) > LineFollow::POST_TAPE_WIDTH){
            LineFollow::postDetected = true;
        }
        return edgeXPos - target;
    }

    /**
     * Runs the PID to follow the tape
     */
    void LineFollow::followTape()
    {
        float error = getLinePositionError(LineFollow::followRightEdge); // get current error
        if(LineFollow::postDetected){ return; }

        LineFollow::errorHistory.push(error); // add current error to errorQueue
        if(LineFollow::errorHistory.size() > LineFollow::ERROR_HISTORY_SIZE){ // keep queue size at ERROR_HISTORY_SIZE
            LineFollow::errorHistory.pop();
        }

        LineFollow::I_sum += error;
        if(LineFollow::I_sum > LineFollow::maxISum){ //set to max sum to avoid integral windup
            LineFollow::I_sum = LineFollow::maxISum;
        }

        //calc average derivative
        float D_error = (LineFollow::errorHistory.back() - LineFollow::errorHistory.front())/(LineFollow::ERROR_HISTORY_SIZE);

        //adjust speed of both wheels to correct for error
        LineFollow::speedAdj = LineFollow::P_gain*error + LineFollow::I_gain*LineFollow::I_sum + LineFollow::D_gain*D_error;
        //LSpeed += speedAdj;
        //RSpeed -= speedAdj;
    }

    /**
     * 
     */
    void LineFollow::findIR() {

    }

    /**
     * 
     */
    void LineFollow::findPost() {

    }

    /**
     * 
     */
    void LineFollow::findGauntlet() {

    }

    /**
     * 
     */
    void LineFollow::findLine() {

    }
}