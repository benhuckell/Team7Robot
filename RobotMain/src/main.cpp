#include <Wire.h>
#include "libs/Adafruit_GFX.h"
#include "Hardware/HardwareInterface.h"
#include "Paths/Path621Left.h"
#include "Paths/Path534Right.h"
#include "Paths/Path561Left.h"
#include "Paths/Path654Right.h"
#include "Functions/lift.h"
#include "Functions/debugging.h"
#include "Functions/lineFollow.h"

#include "stateController.h"
#include "stm32/HardwareTimer.h"
#include <servo.h>
#include "Hardware/ports.h"


Adafruit_SSD1306 display(-1);

bool rightStart;


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



  if(digitalRead(TOGGLE_SWITCH) == LOW){//switch is accidentally flipped upward
    rightStart = true;
  }else if(digitalRead(TOGGLE_SWITCH) == HIGH){
    rightStart = false;
  }
  bool debugEntered = false;
  int count = 0;
  for(;;) {
    if(digitalRead(TOGGLE_SWITCH) == HIGH){
      debugging_loop();
      debugEntered = true;
    }
    else if(digitalRead(TOGGLE_SWITCH) == LOW && debugEntered){
      digitalWrite(LED_BLUE,LOW);
      lineFollowSetup();
      liftSetup();
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Line Following");
      display.display();
      
      if(rightStart){
        if(analogRead(CONTROL_POT_1) > 512){
            Serial.println("1, >500");
            path534R();
        }
        else{
          Serial.println("1, <500");
          path654R();
        }
      }

      else{//leftStart
        if(analogRead(CONTROL_POT_2) > 512){
           Serial.println("2, >500");
          path621L();
      }
      else{
         Serial.println("2, <500");
        path561L();
      }
      }
    
    }
    //HardwareInterface::i()->update();
    count++;

  }
}

void loop() {}
