/*!
 *
 * @brief FT6336 library by yik3z, adapted from Adafruit FT6206 Library
 */

#ifndef FT6336_LIBRARY
#define FT6336_LIBRARY

#include "Arduino.h"
#include <Wire.h>

#define DEBUG_TOUCHSCREEN                   // flag to print debug messages to serial

#define FT6336_DEFAULT_THRESHOLD 128 // Default threshold for touch detection

//Power Modes of the display
#define FT6336_PWR_MODE_ACTIVE        0x00  // Active Mode
#define FT6336_PWR_MODE_MONITOR       0x01  // Monitor (Low Power) Mode
#define FT6336_PWR_MODE_HIBERNATE     0x03  // Hibernate Mode



enum gest{
  GEST_UP =       0x10,
  GEST_RIGHT =    0x14,
  GEST_DOWN =     0x18,
  GEST_LEFT =     0x1c,
  GEST_ZOOM_IN =  0x48,
  GEST_ZOOM_OUT = 0x49,
  GEST_NONE =     0x00
}; //verified with datasheet


/**************************************************************************/
/*!
    @brief  Helper class that stores a TouchScreen Point with x, y, and z
   coordinates, for easy math/comparison
*/
/**************************************************************************/
class TS_Point {
public:
  TS_Point(void);
  TS_Point(int16_t x, int16_t y, int16_t z, uint8_t gesture);
  

  bool operator==(TS_Point);
  bool operator!=(TS_Point);

  int16_t x; /*!< X coordinate */
  int16_t y; /*!< Y coordinate */
  int16_t z; /*!< Z coordinate (often used for pressure) */
  //gest gesture;
  uint8_t gesture;
};

/**************************************************************************/
/*!
    @brief  Class that stores state and functions for interacting with FT6336
   capacitive touch chips
*/
/**************************************************************************/
class FT6336 {
public:
  FT6336(int8_t intPin = -1, int8_t rstPin = -1);
  boolean begin(uint8_t thresh = FT6336_DEFAULT_THRESHOLD);
  uint8_t touched(void);
  TS_Point getPoint(uint8_t n = 0);
  void setPowerMode(uint8_t);
  bool wakePanel(bool waitForReady = true);
  uint8_t getPowerMode();
  uint8_t getOperatingMode();

  // void autoCalibrate(void);

private:
  void _writeRegister8(uint8_t reg, uint8_t val);
  uint8_t _readRegister8(uint8_t reg);
  void _readData(void);
  
  uint8_t touches;
  uint16_t touchX[2], touchY[2], touchID[2];
  uint8_t touchGesture;
  int8_t rstPin;
  int8_t intPin;
};

#endif // FT6336_LIBRARY
