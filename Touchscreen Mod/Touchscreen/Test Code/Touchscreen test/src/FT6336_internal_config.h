#ifndef FT6336_INTERNAL_CONFIG
#define FT6336_INTERNAL_CONFIG

#define DEBUG_TOUCHSCREEN                   // flag to print debug messages to serial

#define FT6336_ADDR                   0x38  // Touchscreen I2C address
#define FT6336_REG_NUMTOUCHES         0x02  // Number of touch points

#define FT6336_NUM_X                  0x33  // Touch X position
#define FT6336_NUM_Y                  0x34  // Touch Y position

// Register Addresses and Values
#define FT6336_REG_MODE               0x00  // Get/Set Device mode, either WORKING or FACTORY
//#define FT6336_CALIBRATE              0x02  // Calibrate mode
#define FT6336_WORKMODE               0x00  // Work mode, default
//#define FT6336_FACTORYMODE            0x40  // Factory mode, may be wrong according to datasheet

#define FT6336_REG_THRESHHOLD         0x80  // Get/Set Threshold for touch detection
#define FT6336_REG_AUTO_MONITOR_MODE  0x86  // Get/Set Whether panel automatically switches to monitor mode. default true
#define FT6336_REG_TIME_ENTER_MONITOR 0x87  // Get/Set the time period of switching from Active mode to Monitor mode when there is no touching, in seconds. default 10
#define FT6336_REG_POINTRATE_ACTIVE   0x88  // Get/Set Point report interval in Active Mode, in milliseconds. default 12
#define FT6336_POINTRATE_ACTIVE       60    // Report interval in Active Mode, in milliseconds
#define FT6336_REG_POINTRATE_MONITOR  0x89  // Get/Set Point rate in Monitor Mode, in milliseconds. default 70. Cannot be changed correctly on FT6336
#define FT6336_POINTRATE_MONITOR      200   // Report interval in Monitor Mode, in milliseconds
#define FT6336_REG_CHIPID             0xA3  // Get chip ID
//#define FT6336_CHIPID                 0x64  // FT6336 Chip ID //0x06: FT6336
#define FT6336_REG_INT_MODE           0xA4  // Get/Set Interrupt mode (Polling or Trigger) (default trigger)
#define FT6336_INT_POLL_MODE          0x00  // Interrupt Polling mode (constant low while touch is present)
#define FT6336_INT_TRIG_MODE          0x01  // Interrupt Trigger mode (pulse low for each data ready to be transmitted)
#define FT6336_REG_PWR_MODE           0xA5  // Get/Set Power mode (data TBD, default 1)) (maybe 0;active, 1:monitor, 2:hibernate)
#define FT6336_REG_FIRMVERS           0xA6  // Get Firmware version
#define FT6336_REG_VENDID             0xA8  // Get vendor ID
//#define FT6336_VENDID                 0x11  // FT6336 FocalTech's vendor ID
#define FT6336_REG_STATE              0xBC  // Get/Set current operating mode (data TBD, default 1)



// calibrated for Adafruit 2.8" ctp screen

#endif //FT6336_INTERNAL_CONFIG