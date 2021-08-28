#ifndef WATCHY_7_SEG_H
#define WATCHY_7_SEG_H

#include <Watchy.h>
/*
#include "Seven_Segment10pt7b.h"
#include "DSEG7_Classic_Regular_15.h"
#include "DSEG7_Classic_Bold_25.h"
#include "DSEG7_Classic_Regular_39.h" //used for weather which idw
*/
#include "DIN_Black35pt7b.h"
#include "DIN_Medium10pt7b.h"
#include "icons.h"

class Watchy7SEG : public Watchy{
    public:
        Watchy7SEG();
        void drawWatchFace();
        void drawTime();
        void drawDate();
        //void drawSteps();
        //void drawWeather(); //who even wants weather
        //void drawTemperature();   //temp is messed up idk why. Seems like hardware issue
        void drawBatteryBar();
        void drawBleWiFi();
};

#endif