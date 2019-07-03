#include "Arduino.h"
#include "StateLoops/lineFollow.h"
#include "configGlobals.h"
<<<<<<< HEAD
#include <Wire.h>
#include <queue>
#include <Adafruit_SSD1306.h>

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

#define MIN_PWM_THRES 0 //not used
#define MAX_PWM_THRES 800 //not used
#define MID_PWM_THRES (MAX_PWM_THRES - MIN_PWM_THRES)/2 //not used
#define T_PWM 2000 //2000 clock pulses
#define PWM_FREQ 100000 //100KHz
 
//Globals
float LSpeed = 50;
float RSpeed = 50;
const int NUM_QRD_SENSORS = 4; //number of active QRD sensors
float QRD_Calib_Max[NUM_QRD_SENSORS];
float QRD_Calib_Min[NUM_QRD_SENSORS];
float EDGE_QRD_VAL[NUM_QRD_SENSORS]; //will almost certainly need to change maybe make non static
const int P_gain = 100; // K_p
const int I_gain = 0; // K_i
const int D_gain = 0; // K_d
float I_sum = 0; //cummulative sum
const float maxISum = 2; //max sum to avoid integral windup
std::queue<float> errorHistory; //holds history of recorded line errors
const int ERROR_HISTORY_SIZE = 10; //max size of error queue
const float POST_TAPE_WIDTH = 4;
bool postDetected = false;
=======


namespace StateLoops {
    

    enum lineFollowStates{online,offline} lineFollowState;
    
<<<<<<< HEAD
    LineFollow(){
        //initial EDGE_QRD_VALs
        QRD_Calib_Max[0] = 250;
        QRD_Calib_Max[1] = 250;
        QRD_Calib_Max[2] = 250;
        QRD_Calib_Max[3] = 250;

        QRD_Calib_Min[0] = 0;
        QRD_Calib_Min[1] = 0;
        QRD_Calib_Min[2] = 0;
        QRD_Calib_Min[3] = 0;
    }

    //sets the Motor speeds based on the global variables 'LSpeed' and 'RSpeed'
    void LineFollow::update() {
        int t_pulse;
        //limit power to 100
        if(LSpeed > 100){
            LSpeed = 100;
        }
        if(RSpeed > 100){
            RSpeed = 100;
        }
        //Left Motor
        t_pulse = T_PWM * ( 1.0 - LSpeed/100);
        if(LSpeed >= 0){
            //fowards
            pwm_start(PWM_LMOTOR_FORWARDS, PWM_FREQ, T_PWM, int(t_pulse), 0);
            pwm_start(PWM_LMOTOR_BACKWARDS, PWM_FREQ, T_PWM, 0, 0);
        }
        else if(LSpeed < 0){
            //backwards
            pwm_start(PWM_LMOTOR_FORWARDS, PWM_FREQ, T_PWM, 0, 0);
            pwm_start(PWM_LMOTOR_BACKWARDS, PWM_FREQ, T_PWM, int(t_pulse), 0);
        }

        //Right Motor
        t_pulse = T_PWM * (1.0 - RSpeed/100);
        if(RSpeed >= 0){
            //forwards
            pwm_start(PWM_RMOTOR_FORWARDS, PWM_FREQ, T_PWM, int(t_pulse), 0);
            pwm_start(PWM_RMOTOR_BACKWARDS, PWM_FREQ, T_PWM, 0, 0);
        }
        else if(RSpeed < 0){
            //backwards
            pwm_start(PWM_RMOTOR_FORWARDS, PWM_FREQ, T_PWM, int(t_pulse), 0);
            pwm_start(PWM_RMOTOR_BACKWARDS, PWM_FREQ, T_PWM, 0, 0);
        }
    }

    // Returns the current amount of line following error
    int LineFollow::getLinePositionError(bool followRightEdge)
    {
        // Read from QRD sensors
        float QRD_Out[NUM_QRD_SENSORS];


        for(int n = 0; n < NUM_QRD_SENSORS; n++)
        {
            // Update Select output ports to increment upwards in binary with 'n' -> 000, 001, 010, etc
            digitalWrite(Select_0, n & 1);
            digitalWrite(Select_1, (n&2)!=0);
            digitalWrite(Select_2, (n&4)!=0);

            QRD_Out[n] = analogRead(QRD_IN1); //read from first multiplexer
            QRD_Out[NUM_QRD_SENSORS/2 + n] = analogRead(QRD_IN2); //read from second multiplexer
        }

        //Find largest and smallest sensor readings
        for(int i = 0; i < NUM_QRD_SENSORS; i++){
            QRD_Calib_Max[i] = QRD_Out[0];
            QRD_Calib_Min[i] = QRD_Out[0];
        }
        for(int i = 1; i < NUM_QRD_SENSORS; i++)
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
        if(followRightEdge){ 
                //Find right edge
            for(i = NUM_QRD_SENSORS-1; ((QRD_Out[i]-QRD_Calib_Min[i]) > EDGE_QRD_VAL[i]-QRD_Calib_Min[i]) && (i >= 0); i--)
            ;//intentionally blank for loop
            if(i == NUM_QRD_SENSORS-1){ rightEdgeXVal = 0 + (NUM_QRD_SENSORS-1)*0.5; } //handle case where line is directly below rightmost sensor
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
            for(i = 0; (QRD_Out[i] > EDGE_QRD_VAL[i]) && (i < NUM_QRD_SENSORS); i++)
            ;//intentionally empty for loop
            if(i == 0){ leftEdgeXVal = 0 - (NUM_QRD_SENSORS-1)*0.5; } //handle case where line is directly below leftmost sensor
            else{
            //interpolate and subtract (NUM_QRD_SENSORS-1)/2 to put a zero x value in the middle of the sensor array
            leftEdgeXVal = i - (EDGE_QRD_VAL[i] - QRD_Out[i])/(QRD_Out[i-1] - QRD_Out[i]) - (NUM_QRD_SENSORS-1)*0.5;
            }
            edgeXPos = leftEdgeXVal;
            target = -1;
        }
        if((rightEdgeXVal - leftEdgeXVal) > POST_TAPE_WIDTH){
            postDetected = true;
        }
        return edgeXPos - target;
    }

    //runs a PID to follow the tape
    void LineFollow::followTape(bool followRightEdge){
        
        bool followRightEdge = false;
        float error = getLinePositionError(followRightEdge); // get current error
        if(postDetected){
            return;
        }
        errorHistory.push(error); // add current error to errorQueue
        if(errorHistory.size() > ERROR_HISTORY_SIZE){ // keep queue size at ERROR_HISTORY_SIZE
            errorHistory.pop();
        }

        I_sum += error;
        if(I_sum > maxISum){ //set to max sum to avoid integral windup
            I_sum = maxISum;
        }

        //calc average derivative
        float D_error = (errorHistory.back() - errorHistory.front())/(ERROR_HISTORY_SIZE);

        //adjust speed of both wheels to correct for error
        float speedAdj = P_gain*error + I_gain*I_sum + D_gain*D_error;
        LSpeed += speedAdj;
        RSpeed -= speedAdj;
        setMotorSpeed();
=======
    void LineFollow::loop(){
        //MainState::i()->setState(avengerCollecting);
        display.println("Line Following");
        //delay(1000);
        return;
>>>>>>> 75cc3732e2d12debc6c8c0dca9018ac8c8d620d2
    }
    
    void LineFollow::findIR() {

    }

    void LineFollow::findPost() {

    }

    void LineFollow::findGauntlet() {

    }

    void LineFollow::findLine() {

    }
}