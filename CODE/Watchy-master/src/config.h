#ifndef CONFIG_H
#define CONFIG_H

//pins
#define SDA 21
#define SCL 22
#define ADC_PIN 33
#define RTC_PIN GPIO_NUM_27
#define CS 5
#define DC 10
#define RESET 9
#define BUSY 19
#define VIB_MOTOR_PIN 13
#define MENU_BTN_PIN 26
#define BACK_BTN_PIN 25
#define UP_BTN_PIN 32
#define DOWN_BTN_PIN 4
#define MENU_BTN_MASK GPIO_SEL_26
#define BACK_BTN_MASK GPIO_SEL_25
#define UP_BTN_MASK GPIO_SEL_32
#define DOWN_BTN_MASK GPIO_SEL_4
#define ACC_INT_MASK GPIO_SEL_14
#define BTN_PIN_MASK MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK
//display
#define DISPLAY_WIDTH 200
#define DISPLAY_HEIGHT 200
#define DISPLAY_REFRESH_INTERVAL 300    //max refresh interval for the display
//weather api
#define CITY_NAME "SINGAPORE" //if your city name has a space, replace with '+'
#define COUNTRY_CODE "SG"

//#define OPENWEATHERMAP_APIKEY "f058fe1cad2afe8e2ddc5d063a64cecb" //defined in sensitive_config.h
#define OPENWEATHERMAP_URL "http://api.openweathermap.org/data/2.5/weather?q="
#define TEMP_UNIT "metric" //use "imperial" for Fahrenheit"
#define WEATHER_UPDATE_INTERVAL 30 //minutes

//wifi
#define WIFI_AP_TIMEOUT 60
#define WIFI_AP_SSID "Watchy AP"  //used for WiFiManager AP
//WIFI_SSID and WIFI_PASSWORD defined in sensitive_config.h

//menu
#define WATCHFACE_STATE -1
#define MAIN_MENU_STATE 0
#define APP_STATE 1
//#define FW_UPDATE_STATE 2

//stopwatch
#define STOPWATCH_TIME_X_0 50
#define STOPWATCH_TIME_Y_0 100
#define STOPWATCH_INTERVAL DISPLAY_REFRESH_INTERVAL  //set stopwatch dsiplay update rate to display refresh rate

//apps 
/*
#define STOPWATCH_STATE 1
#define TIMER_STATE 2
#define BATT_STATE 3
#define ACC_STATE 3
#define TEMPERATURE_STATE 3
*/
#define MENU_HEIGHT 30
#define MENU_LENGTH 6
//set time
#define SET_HOUR 0
#define SET_MINUTE 1
#define SET_YEAR 2
#define SET_MONTH 3
#define SET_DAY 4
#define YEAR_OFFSET 1970
#define NTP_TIME_SYNC_INTERVAL 10080 // once every 7 days. ie 7days * 24h * 60min
#define GMT_OFFSET_SEC 28800 // set time zone to central standard time
                                // ie UTC -6 * 60 * 60 = -21600
#define DAYLIGHT_OFFSET_SEC 0 // if observing Daylight saving time 3600 otherwise 0
#define NTP_SERVER "pool.ntp.org"
//BLE OTA
#define BLE_DEVICE_NAME "Watchy BLE OTA"
#define WATCHFACE_NAME "Watchy 7 Segment"
#define SOFTWARE_VERSION_MAJOR 1
#define SOFTWARE_VERSION_MINOR 0
#define SOFTWARE_VERSION_PATCH 0
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0

#endif