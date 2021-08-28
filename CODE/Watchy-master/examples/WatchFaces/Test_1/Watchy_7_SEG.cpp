#include "Watchy_7_SEG.h"

bool DARKMODE = true; //TESTING whether the menu can work

//const uint8_t BATTERY_SEGMENT_WIDTH = 7;
//const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
//const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t BATTERY_BAR_HEIGHT = 5; 
//const uint8_t WEATHER_ICON_WIDTH = 48;
//const uint8_t WEATHER_ICON_HEIGHT = 32;
const uint8_t DATE_TIME_X_0 = 15;
const uint8_t TIME_Y_0 = 110; 
const uint8_t DATE_Y_0 = 144;
const uint8_t TEMPERATURE_X_0 = 145;
const uint8_t TEMPERATURE_Y_0 = 175;

Watchy7SEG::Watchy7SEG(){} //constructor

void Watchy7SEG::drawWatchFace(){
    DARKMODE = getDarkModeStatus();
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    drawTime();
    drawDate();
    //drawSteps();
    //drawWeather();
    //drawTemperature();
    drawBatteryBar();
    drawBleWiFi();
    syncNtpTime();
    
    
}

void Watchy7SEG::drawTime(){
    display.setFont(&DIN_Black35pt7b);

    display.setCursor(DATE_TIME_X_0 - 5, TIME_Y_0);
    if(currentTime.Hour < 10){
        display.print("0");
    }
    display.print(currentTime.Hour);
    display.print(":");
    if(currentTime.Minute < 10){
        display.print("0");
    }  
    display.println(currentTime.Minute);  
}

void Watchy7SEG::drawDate(){

    //divider line
    display.fillRect(DATE_TIME_X_0, TIME_Y_0 + 10, 165, 3, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    display.setFont(&DIN_Medium10pt7b);
    String dayOfWeek = dayStr(currentTime.Wday);
    display.setCursor(DATE_TIME_X_0, DATE_Y_0);
    display.println(dayOfWeek);

    display.setCursor(113, DATE_Y_0);
    if(currentTime.Day < 10){
    display.print("0");      
    }     
    display.print(currentTime.Day);

    display.print("."); 
    if(currentTime.Month < 10){
    display.print("0");      
    }
    display.print(currentTime.Month);
    display.print("."); 
    uint16_t yearTwoDigits = currentTime.Year + YEAR_OFFSET - 2000; //to get '21 hehe
    display.println(yearTwoDigits); // offset from 1970, since year is stored in uint8_t

    
}

/*
void Watchy7SEG::drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    uint32_t stepCount = sensor.getCounter();
    display.drawBitmap(10, 165, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(35, 190);
    display.println(stepCount);
}
*/
void Watchy7SEG::drawBatteryBar(){
    //battery bar
    display.fillRect(0, 0, DISPLAY_WIDTH, BATTERY_BAR_HEIGHT, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE); //clear battery bar; IS THIS EVEN REQUIRED?
    float VBAT = getBatteryVoltage();
    //https://github.com/G6EJD/LiPo_Battery_Capacity_Estimator/blob/master/ReadBatteryCapacity_LIPO.ino as linked by Huey's github
    uint8_t percentage = 100;
    if (VBAT > 4.19) percentage = 100;
    else if (VBAT <= 3.50) percentage = 0;
    else percentage = 2808.3808 * pow(VBAT, 4) - 43560.9157 * pow(VBAT, 3) + 252848.5888 * pow(VBAT, 2) - 650767.4615 * VBAT + 626532.5703;
    uint8_t batteryBarWidth = percentage * 2;
    display.fillRect(0, 0, batteryBarWidth, BATTERY_BAR_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}
void Watchy7SEG::drawBleWiFi(){
    if(BLE_CONFIGURED){ 
        display.drawBitmap(150, 20, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    if(WIFI_CONFIGURED){ 
        display.drawBitmap(168, 20, wifi, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

/* function disabled till I figure out what's the issue with the wrong temp
void Watchy7SEG::drawTemperature(){
    weatherData currentWeather = getWeatherData(false); //get temp from RTC
    int8_t temperature = currentWeather.temperature;
    display.setFont(&DIN_Medium10pt7b);

    display.setCursor(TEMPERATURE_X_0, TEMPERATURE_Y_0);
    display.print(temperature);
    display.println(" C");
    //display.drawBitmap(TEMPERATURE_X_CENTER, TEMPERATURE_Y_0, strcmp(TEMP_UNIT, "metric") == 0 ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}
*/


/* I don't want weather
void Watchy7SEG::drawWeather(){

    weatherData currentWeather = getWeatherData();

    int8_t temperature = currentWeather.temperature;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;   

    display.setFont(&DSEG7_Classic_Regular_39);
    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
    if(159 - w - x1 > 87){
        display.setCursor(159 - w - x1, 150);
    }else{
        display.setFont(&DSEG7_Classic_Bold_25);
        display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
        display.setCursor(159 - w - x1, 136);
    }
    display.println(temperature);
    display.drawBitmap(165, 110, strcmp(TEMP_UNIT, "metric") == 0 ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    const unsigned char* weatherIcon;

    //https://openweathermap.org/weather-conditions
    if(weatherConditionCode > 801){//Cloudy
    weatherIcon = cloudy;
    }else if(weatherConditionCode == 801){//Few Clouds
    weatherIcon = cloudsun;  
    }else if(weatherConditionCode == 800){//Clear
    weatherIcon = sunny;  
    }else if(weatherConditionCode >=700){//Atmosphere
    weatherIcon = cloudy; 
    }else if(weatherConditionCode >=600){//Snow
    weatherIcon = snow;
    }else if(weatherConditionCode >=500){//Rain
    weatherIcon = rain;  
    }else if(weatherConditionCode >=300){//Drizzle
    weatherIcon = rain;
    }else if(weatherConditionCode >=200){//Thunderstorm
    weatherIcon = rain; 
    }else
    return;
    display.drawBitmap(145, 158, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}
*/
