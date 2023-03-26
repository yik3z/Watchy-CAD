#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "FT6336.h"

#define DEBUG_TOUCHSCREEN

#define TS_RESET_PIN     GPIO_NUM_23
#define TS_INTERRUPT_PIN GPIO_NUM_34

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
RTC_DATA_ATTR uint32_t counter = 0;

// void IRAM_ATTR touchscreenInt() {
//   screenTouchedFlag = 1;
//   touchTimeDiff = millis() - lastInterrupt;
//   lastInterrupt = millis();
//   //Serial.println("Int!");
// }

void setup(void)
{

  Wire.begin(); // GPIO 22 (SCL) and GPIO 21 (SDA)
  pinMode(TS_INTERRUPT_PIN, INPUT);
  Serial.begin(115200);
  if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_EXT0){ // not a touchscreen interrupt, i.e. it's the initial boot
    digitalWrite(TS_RESET_PIN,LOW);
    pinMode(TS_RESET_PIN, OUTPUT);
    delay(6);
    digitalWrite(TS_RESET_PIN,HIGH);
    if (!ts.begin(200)) { 
      Serial.println("Unable to start touchscreen.");
    } 
    else { 
      Serial.println("Touchscreen started."); 
    }
  }
  Serial.println(analogRead(TS_INTERRUPT_PIN));
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

    // rotate coordinate system
    // flip it around to match the screen.
    //p.x = map(p.x, 0, 240, 240, 0);
    //p.y = map(p.y, 0, 320, 320, 0);

    //screenTouchedFlag = 0;
    if(ts.touched()){    
      TS_Point p = ts.getPoint(); 
      Serial.print("Screen touched. x: ");
      Serial.print(p.x);
      Serial.print(" | y: ");
      Serial.print(p.y);
      Serial.print(" | Gesture: "); // gesture never seems to work
      Serial.print(p.gesture);
      Serial.println();
      counter++;
    // Serial.print("Pwr Mode: ");
    // Serial.print(ts.getPowerMode());
    // Serial.print(" | OpMode: "); // trying to figure out what the power modes mean
    // Serial.println(ts.getOperatingMode());
    }
    if (counter > 5){
      counter = 0;
      ts.setPowerMode(FT6336_PWR_MODE_HIBERNATE);
      Serial.print("Hibernating Display. Power Mode: ");
      Serial.println(ts.getPowerMode());    
      delay(5000);
      Serial.print("Waking Display: ");
      if(ts.wakePanel()){
        Serial.println("Wake Successful");
        ts.begin(); // init after wake from hibernate
      }else{
        Serial.println("Wake Failed");
      }
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
  while(digitalRead(TS_INTERRUPT_PIN)==LOW); //wait for int to clear
  //Serial.println(analogRead(TS_INTERRUPT_PIN));
  ts.setPowerMode(FT6336_PWR_MODE_MONITOR);
  esp_sleep_enable_ext0_wakeup(TS_INTERRUPT_PIN, LOW); //enable deep sleep wake on touchscreen interrupt (TODO: change to ext1)
  esp_deep_sleep_start();

}

void loop()
{
  
}
