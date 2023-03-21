#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "FT3267.h"

#define DEBUG_TOUCHSCREEN

#define RESET_PIN     GPIO_NUM_34
#define INTERRUPT_PIN GPIO_NUM_19
#define ACTIVE_POLLING_RATE   25
#define MONITOR_POLLING_RATE  5

// The FT3267 uses hardware I2C (SCL (22)/SDA (21))
Adafruit_FT3267 ts = Adafruit_FT3267(INTERRUPT_PIN, RESET_PIN);

//boolean RecordOn = false;
//
//#define FRAME_X 200
//#define FRAME_Y 200
//#define FRAME_W 100
//#define FRAME_H 50

long lastInterrupt = 0; // time when the last interrupt was fired
long touchTimeDiff = 0; // time difference between two consecutive interrupts
volatile bool screenTouchedFlag = 0;  // flag for interrupt
int counter = 0;

void IRAM_ATTR touchscreenInt() {
  screenTouchedFlag = 1;
  touchTimeDiff = millis() - lastInterrupt;
  lastInterrupt = millis();
  //Serial.println("Int!");
}

void setup(void)
{
  pinMode(INTERRUPT_PIN, INPUT);
  //pinMode(RESET_PIN, INPUT);
  Serial.begin(9600);
  //tft.begin();
  if (!ts.begin(200)) { 
    Serial.println("Unable to start touchscreen.");
  } 
  else { 
    Serial.println("Touchscreen started."); 
  }

  attachInterrupt(INTERRUPT_PIN, touchscreenInt, RISING);

  //esp_sleep_enable_ext0_wakeup(INTTERUPT_PIN, 0); //enable deep sleep wake on touchscreen interrupt
  //esp_deep_sleep_start();

}

void loop()
{
  // See if there's any  touch data for us
  //if (ts.touched())
  if(screenTouchedFlag)
  {   
    // Serial.print("Interrupt interval: ");
    // Serial.println(touchTimeDiff);
    // screenTouchedFlag = 0;
    // Retrieve a point  
    delay(80); //emulate watchy powering up
    TS_Point p = ts.getPoint(); 
    // rotate coordinate system
    // flip it around to match the screen.
    //p.x = map(p.x, 0, 240, 240, 0);
    //p.y = map(p.y, 0, 320, 320, 0);

    screenTouchedFlag = 0;

    Serial.print("Screen touched. x: ");
    Serial.print(p.x);
    Serial.print(" | y: ");
    Serial.print(p.y);
    Serial.print(" | Gesture: ");
    Serial.print(p.gesture);
    Serial.println();
    Serial.print("Pwr Mode: "); // trying to figure out what the power modes mean
    Serial.print(ts.getPowerMode());
    Serial.print(" | OpMode: "); // trying to figure out what the power modes mean
    Serial.println(ts.getOperatingMode());
    // counter++;
    // if(counter>5){  // try to hibernate the display after 5 touches
    //   counter = 0;
    //   ts.setPowerMode(FT3267_PWR_MODE_HIBERNATE);
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
  }  
  

}
