#include <Wire.h>
#include "libs/Adafruit_GFX.h"
#include "Hardware/HardwareInterface.h"
#include "Paths/Path1.h"
#include "Functions/lift.h"
#include "Functions/debugging.h"
#include "Functions/lineFollow.h"

#include "stateController.h"
#include "stm32/HardwareTimer.h"
#include <servo.h>


Adafruit_SSD1306 display(-1);

void setup() {
  Serial.begin(115200);
  delay(3000);

  debug_setup();

  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Working");
  display.display();



  // if(digitalRead(TOGGLE_SWITCH) == 0){//switch is accidentally flipped upward
  //   exit(0);
  // }

  int count = 0;
  for(;;) {

    if(digitalRead(TOGGLE_SWITCH)){
 
      debugging_loop();
    }
    else if(digitalRead(TOGGLE_SWITCH) == 0){
      digitalWrite(LED_BLUE,LOW);
      lineFollowSetup();
      liftSetup();
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Line Following");
      display.display();
      
    
      path1();
    
    }
    //HardwareInterface::i()->update();
    count++;

  }
}

void loop() {}
