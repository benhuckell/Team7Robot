#pragma once

#define LMOTOR_FORWARDS PA_0 //motor ports
#define LMOTOR_BACKWARDS PA_1
#define RMOTOR_FORWARDS PA_2
#define RMOTOR_BACKWARDS PA_3

#define PUSH_BUTTON_1 PB4
#define PUSH_BUTTON_2 PB5

#define CONTROL_POT_1 PA_4
#define CONTROL_POT_2 PA_5


#define WINCH_UP PB_0 //winch inputs
#define WINCH_DOWN PA_7

#define WINCH_ENC_1 PB10
#define WINCH_ENC_2 PB1

#define LENCODER_1 PB15 //interupt 
#define LENCODER_2 PA11 //read
#define RENCODER_1 PA8 //interupt
#define RENCODER_2 PA12 //read //swapped

#define LENCODER_DIRECTION 1
#define RENCODER_DIRECTION -1
#define WINCH_ENCODER_DIRECTION -1

#define CLAW_SERVO PB_1
#define LIM_SWITCH_PIN PB11

#define LED_RED PB9
#define LED_BLUE PB8

#define QRD_IN PA_6 //QRD read port

#define TOGGLE_SWITCH PB3
