/*!
 * @file FT6336.cpp
 *
 * @mainpage FT6336 Library
 *
 * @section intro_sec Introduction
 *
 * This is a library for the Adafruit Capacitive Touch Screens
 *
 * ----> http://www.adafruit.com/products/1947
 *
 * Check out the links above for our tutorials and wiring diagrams
 * This chipset uses I2C to communicate
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License

 * MIT license, all text above must be included in any redistribution
 */

#include "FT6336.h"
#include "FT6336_internal_config.h"

#if defined(__SAM3X8E__)
#define Wire Wire1
#endif

//#define FT6336_DEBUG
//#define I2C_DEBUG

/**************************************************************************/
/*!
    @brief  Instantiates a new FT6336 class
*/
/**************************************************************************/
// I2C, no address adjustments or pins
FT6336::FT6336(int8_t _intPin, int8_t _rstPin) {
  touches = 0;
  intPin = _intPin;
  rstPin = _rstPin;
}

/**************************************************************************/
/*!
    @brief  Setups the I2C interface and hardware, identifies if chip is found
    @param  thresh Optional threshhold-for-touch value, default is
   FT6336_DEFAULT_THRESHOLD but you can try changing it if your screen is
   too/not sensitive.
    @returns True if an FT6336 is found, false on any failure
*/
/**************************************************************************/
boolean FT6336::begin(uint8_t thresh) {
  //delay(400);
  _writeRegister8(FT6336_REG_THRESHHOLD, thresh);
  //_writeRegister8(FT6336_REG_AUTO_MONITOR_MODE, 0);                      // disable auto monitor mode
  _writeRegister8(FT6336_REG_TIME_ENTER_MONITOR, 5);                       // switch to monitor mode after 1s
  _writeRegister8(FT6336_REG_INT_MODE, FT6336_INT_POLL_MODE);              // set to poll mode (1 interrupt per touch event)
  _writeRegister8(FT6336_REG_POINTRATE_ACTIVE, FT6336_POINTRATE_ACTIVE);   // set active mode polling rate in ms
  _writeRegister8(FT6336_REG_POINTRATE_MONITOR, FT6336_POINTRATE_MONITOR); // set monitor mode polling rate to in ms
  if(_readRegister8(FT6336_REG_VENDID) == 255) {
    #ifdef DEBUG_TOUCHSCREEN
    Serial.println("Cannot communicate with the touchscreen.");
    #endif
    return false;
  }
  #ifdef DEBUG_TOUCHSCREEN
  Serial.print("Vend ID: 0x");
  Serial.println(_readRegister8(FT6336_REG_VENDID), HEX);  //0x23
  Serial.print("Chip ID: 0x");
  Serial.println(_readRegister8(FT6336_REG_CHIPID), HEX);  //0x33
  // Serial.print("Firm V: ");
  // Serial.println(_readRegister8(FT6336_REG_FIRMVERS)); //16
  Serial.print("Point Rate (Active): ");
  Serial.println(_readRegister8(FT6336_REG_POINTRATE_ACTIVE)); //12??
  Serial.print("Point Rate (Monitor): ");
  Serial.println(_readRegister8(FT6336_REG_POINTRATE_MONITOR));  //70??
  Serial.print("Auto Monitor: ");
  Serial.println(_readRegister8(FT6336_REG_AUTO_MONITOR_MODE));
  Serial.print("Switch to Monitor Delay: ");
  Serial.println(_readRegister8(FT6336_REG_TIME_ENTER_MONITOR));
  Serial.print("Interrupt Mode: ");
  Serial.println(_readRegister8(FT6336_REG_INT_MODE));
  Serial.print("Power Mode: ");
  Serial.println(_readRegister8(FT6336_REG_PWR_MODE));
  Serial.print("Operating Mode(?): ");
  Serial.println(_readRegister8(FT6336_REG_STATE));
  Serial.print("Touch Thresh: ");
  Serial.println(_readRegister8(FT6336_REG_THRESHHOLD));
  // dump all registers
  // for (int16_t i = 0; i < 0x10; i++) {
  //   Serial.print("I2C $");
  //   Serial.print(i, HEX);
  //   Serial.print(" = 0x");
  //   Serial.println(_readRegister8(i), HEX);
  // }
  #endif //DEBUG_TOUCHSCREEN

  return true;
}

/**************************************************************************/
/*!
    @brief  Determines if and how many touches detected through I2C
    @returns Number of touches detected, can be 0, 1 or 2
*/
/**************************************************************************/
uint8_t FT6336::touched(void) {
  uint8_t n = _readRegister8(FT6336_REG_NUMTOUCHES);
  if (n > 2) {
    n = 0;
  }
  return n;
}

// /**************************************************************************/
// /*!
//     @brief  Determines if there are any touches detected by reading the INT pin
//     @returns Whether a touch is detected
// */
// /**************************************************************************/
// bool FT6336::touchedInt(void) {
//   //TODO
//   if(intPin==-1) return 0; //pin not defined
//   bool touched = !digitalRead(rstPin);  //TODO: should be _intPin right??
//   return touched;
// }

// Returns the operating mode of the touchscreen
uint8_t FT6336::getOperatingMode(void) {
  return _readRegister8(FT6336_REG_STATE);
}

/**************************************************************************/
/*!
    @brief  Queries the chip and retrieves a point data
    @param  n The # index (0 or 1) to the points we can detect. In theory we can
   detect 2 points but we've found that you should only use this for
   single-touch since the two points cant share the same half of the screen.
    @returns {@link TS_Point} object that has the x and y coordinets set. If the
   z coordinate is 0 it means the point is not touched. If z is 1, it is
   currently touched.
*/
/**************************************************************************/
TS_Point FT6336::getPoint(uint8_t n) {
  _readData();
  if ((touches == 0) || (n > 1)) {
    return TS_Point(0, 0, 0, 0);
  } else {
    return TS_Point(touchX[n], touchY[n], 1, touchGesture);
  }
}


// Returns the power mode of the touchscreen
uint8_t FT6336::getPowerMode(void) {
  uint8_t pwrMode = _readRegister8(FT6336_REG_PWR_MODE); 
  return pwrMode;
}

/**************************************************************************/
/*!
    @brief  Sets the panel power mode.
    @returns None
*/
/**************************************************************************/
void FT6336::setPowerMode(uint8_t pwrMode) {
  if ((pwrMode == FT6336_PWR_MODE_HIBERNATE) && (rstPin == -1)) {
    #ifdef DEBUG_TOUCHSCREEN 
    Serial.println("Hibernate command ignored. No reset pin defined.");
    #endif
    return;
  }
  _writeRegister8(FT6336_REG_PWR_MODE,pwrMode);
}

/**************************************************************************/
/*!
    @brief  Wakes panel from hibernate mode.
    @param waitForReady Wait for panel to be ready (i.e. block) before returning .
    @returns True if panel is successfully woken up.
*/
/**************************************************************************/
bool FT6336::wakePanel(bool waitForReady) {
  if(rstPin==-1){
    //pin not defined
    #ifdef DEBUG_TOUCHSCREEN 
    Serial.println("Cannot wake panel. No reset pin defined");
    #endif
    return false;
  } 
  else{
    // digitalWrite(rstPin, LOW);
    // pinMode(rstPin, OUTPUT);
    // delay(2); //between 0.5-1ms for int or >1ms for rst
    // pinMode(rstPin, INPUT);
    digitalWrite(rstPin, LOW);
    pinMode(rstPin, OUTPUT);
    delay(6); // 1ms for rstPin or between 0.5-1ms for intPin
    pinMode(rstPin, INPUT);
    if(waitForReady){
      #if defined(ESP8266) || defined(ESP32)
      esp_sleep_enable_timer_wakeup(400000); // post reset delay
      esp_light_sleep_start();
      #else
      delay(300); // post reset delay
      #endif
      begin();
      if(getPowerMode() == FT6336_PWR_MODE_ACTIVE) return true;
    }
      
  }
  return false; //change to true when it works
}

/************ lower level i/o **************/

/**************************************************************************/
/*!
    @brief  Reads the bulk of data from captouch chip. Fill in {@link touches},
   {@link touchX}, {@link touchY} and {@link touchID} with results
*/
/**************************************************************************/
void FT6336::_readData(void) {

  uint8_t i2cdat[16];
  Wire.beginTransmission(FT6336_ADDR);
  Wire.write((byte)0);
  Wire.endTransmission();

  Wire.requestFrom((byte)FT6336_ADDR, (byte)16);
  for (uint8_t i = 0; i < 16; i++)
    i2cdat[i] = Wire.read();

// #ifdef FT6336_DEBUG
//   for (int16_t i = 0; i < 16; i++) {
//     Serial.print("I2C $");
//     Serial.print(i, HEX);
//     Serial.print(" = 0x");
//     Serial.println(i2cdat[i], HEX);
//   }
// #endif
  touchGesture = i2cdat[0x01];  //read save gesture
  touches = i2cdat[0x02];       //save number of touches
  if ((touches > 2) || (touches == 0)) {
    touches = 0;
  }

// #ifdef FT6336_DEBUG
//   Serial.print("# Touches: ");
//   Serial.println(touches);

//   for (uint8_t i = 0; i < 16; i++) {
//     Serial.print("0x");
//     Serial.print(i2cdat[i], HEX);
//     Serial.print(" ");
//   }
//   Serial.println();
//   if (i2cdat[0x01] != 0x00) {
//     Serial.print("Gesture #");
//     Serial.println(i2cdat[0x01]);
//   }
// #endif
  
  for (uint8_t i = 0; i < 2; i++) {
    touchX[i] = i2cdat[0x03 + i * 6] & 0x0F;
    touchX[i] <<= 8;
    touchX[i] |= i2cdat[0x04 + i * 6];
    touchY[i] = i2cdat[0x05 + i * 6] & 0x0F;
    touchY[i] <<= 8;
    touchY[i] |= i2cdat[0x06 + i * 6];
    touchID[i] = i2cdat[0x05 + i * 6] >> 4;
  }

// #ifdef FT6336_DEBUG
//   Serial.println();
//   for (uint8_t i = 0; i < touches; i++) {
//     Serial.print("ID #");
//     Serial.print(touchID[i]);
//     Serial.print("\t(");
//     Serial.print(touchX[i]);
//     Serial.print(", ");
//     Serial.print(touchY[i]);
//     Serial.print(") ");
//   }
//   Serial.println();
// #endif
}

uint8_t FT6336::_readRegister8(uint8_t reg) {
  uint8_t x;
  // use i2c
  Wire.beginTransmission(FT6336_ADDR);
  Wire.write((byte)reg);
  Wire.endTransmission();

  Wire.requestFrom((byte)FT6336_ADDR, (byte)1);
  x = Wire.read();

// #ifdef I2C_DEBUG
//   Serial.print("$");
//   Serial.print(reg, HEX);
//   Serial.print(": 0x");
//   Serial.println(x, HEX);
// #endif

  return x;
}

void FT6336::_writeRegister8(uint8_t reg, uint8_t val) {
  // use i2c
  Wire.beginTransmission(FT6336_ADDR);
  Wire.write((byte)reg);
  Wire.write((byte)val);
  Wire.endTransmission();
}

/*

// DONT DO THIS - REALLY - IT DOESNT WORK
void FT6336::autoCalibrate(void) {
 _writeRegister8(FT06_REG_MODE, FT6336_REG_FACTORYMODE);
 delay(100);
 //Serial.println("Calibrating...");
 _writeRegister8(FT6336_REG_CALIBRATE, 4);
 delay(300);
 for (uint8_t i = 0; i < 100; i++) {
   uint8_t temp;
   temp = _readRegister8(FT6336_REG_MODE);
   Serial.println(temp, HEX);
   //return to normal mode, calibration finish
   if (0x0 == ((temp & 0x70) >> 4))
     break;
 }
 delay(200);
 //Serial.println("Calibrated");
 delay(300);
 _writeRegister8(FT6336_REG_MODE, FT6336_REG_FACTORYMODE);
 delay(100);
 _writeRegister8(FT6336_REG_CALIBRATE, 5);
 delay(300);
 _writeRegister8(FT6336_REG_MODE, FT6336_REG_WORKMODE);
 delay(300);
}
*/

/****************/

/**************************************************************************/
/*!
    @brief  Instantiates a new FT6336 class with x, y and z set to 0 by default
*/
/**************************************************************************/
TS_Point::TS_Point(void) { x = y = z = 0; }

/**************************************************************************/
/*!
    @brief  Instantiates a new FT6336 class with x, y and z set by params.
    @param  _x The X coordinate
    @param  _y The Y coordinate
    @param  _z The Z coordinate
*/
/**************************************************************************/

TS_Point::TS_Point(int16_t _x, int16_t _y, int16_t _z, uint8_t _gesture) {
  x = _x;
  y = _y;
  z = _z;
  gesture = _gesture;
}

/**************************************************************************/
/*!
    @brief  Simple == comparator for two TS_Point objects
    @returns True if x, y and z are the same for both points, False otherwise.
*/
/**************************************************************************/
bool TS_Point::operator==(TS_Point p1) {
  return ((p1.x == x) && (p1.y == y) && (p1.z == z));
}

/**************************************************************************/
/*!
    @brief  Simple != comparator for two TS_Point objects
    @returns False if x, y and z are the same for both points, True otherwise.
*/
/**************************************************************************/
bool TS_Point::operator!=(TS_Point p1) {
  return ((p1.x != x) || (p1.y != y) || (p1.z != z));
}
