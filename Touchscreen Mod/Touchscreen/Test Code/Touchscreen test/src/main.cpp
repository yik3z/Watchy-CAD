#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "FT6336.h"

#define DEBUG_TOUCHSCREEN

#define TS_RESET_PIN     GPIO_NUM_34
#define TS_INTERRUPT_PIN GPIO_NUM_4

// The FT6336 uses hardware I2C (SCL (22)/SDA (21))
FT6336 ts = FT6336(TS_INTERRUPT_PIN, TS_RESET_PIN);

//boolean RecordOn = false;
//
//#define FRAME_X 200
//#define FRAME_Y 200
//#define FRAME_W 100
//#define FRAME_H 50

// long lastInterrupt = 0; // time when the last interrupt was fired
// long touchTimeDiff = 0; // time difference between two consecutive interrupts
// volatile bool screenTouchedFlag = 0;  // flag for interrupt
// int counter = 0;

// void IRAM_ATTR touchscreenInt() {
//   screenTouchedFlag = 1;
//   touchTimeDiff = millis() - lastInterrupt;
//   lastInterrupt = millis();
//   //Serial.println("Int!");
// }

void setup(void)
{
  Wire.begin();
  //pinMode(TS_INTERRUPT_PIN, INPUT);
  //pinMode(RESET_PIN, INPUT);
  Serial.begin(115200);
  if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_EXT1){ // not a touchscreen interrupt, i.e. it's the initial boot
    if (!ts.begin(200)) { 
      Serial.println("Unable to start touchscreen.");
    } 
    else { 
      Serial.println("Touchscreen started."); 
    }
  }

  //attachInterrupt(TS_INTERRUPT_PIN, touchscreenInt, RISING);

  //esp_sleep_enable_ext0_wakeup(INTTERUPT_PIN, 0); //enable deep sleep wake on touchscreen interrupt
  //esp_deep_sleep_start();
  //if(screenTouchedFlag)
  //{   
    // Serial.print("Interrupt interval: ");
    // Serial.println(touchTimeDiff);
    // screenTouchedFlag = 0;
    // Retrieve a point  
    //delay(80); //emulate watchy powering up
    TS_Point p = ts.getPoint(); 
    // rotate coordinate system
    // flip it around to match the screen.
    //p.x = map(p.x, 0, 240, 240, 0);
    //p.y = map(p.y, 0, 320, 320, 0);

    //screenTouchedFlag = 0;
    if(ts.touched()){
    Serial.print("Screen touched. x: ");
    Serial.print(p.x);
    Serial.print(" | y: ");
    Serial.print(p.y);
    Serial.print(" | Gesture: "); // gesture never seems to work
    Serial.print(p.gesture);
    Serial.println();
    // Serial.print("Pwr Mode: ");
    // Serial.print(ts.getPowerMode());
    // Serial.print(" | OpMode: "); // trying to figure out what the power modes mean
    // Serial.println(ts.getOperatingMode());
    }
    // counter++;
    // if(counter>5){  // try to hibernate the display after 5 touches
    //   counter = 0;
    //   ts.setPowerMode(FT6336_PWR_MODE_HIBERNATE);
    //   Serial.println("hibernating display");
    //   delay(200);
    //   Serial.print("Pwr Mode: ");
    //   Serial.print(ts.getPowerMode());
    //   Serial.print(" | OpMode: "); // trying to figure out what the power modes mean
    //   Serial.println(ts.getOperatingMode());
    //   ts.wakePanel();
    //   delay(100);
      
      // Serial.print("Pwr Mode: ");
    // }
  //}  
  while(digitalRead(GPIO_NUM_4)==HIGH); //wait for int to clear
  ts.setPowerMode(FT6336_PWR_MODE_MONITOR);
  esp_sleep_enable_ext1_wakeup(GPIO_SEL_4, ESP_EXT1_WAKEUP_ANY_HIGH);; //enable deep sleep wake on touchscreen interrupt (TODO: change to ext1)
  esp_deep_sleep_start();

}

void loop()
{
  
}
