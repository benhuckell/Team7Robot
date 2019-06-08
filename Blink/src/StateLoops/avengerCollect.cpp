/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"

// Set LED_BUILTIN if it is not defined by Arduino framework
// #define LED_BUILTIN 13

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

}
  
  
  //Define main states robot can have
  enum mainStates{lineFollow,avengerCollect,avengerScore,stoneCollect,stoneScore,defendGauntlet} mainState;


void loop(mainStates mainState)
{
  mainState = lineFollow;
}