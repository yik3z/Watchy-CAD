# Watchy - Fully Open Source E-Paper Watch

![Watchy](https://watchy.sqfmi.com/img/watchy_render.png)

[**Pre-order Watchy!**](https://www.crowdsupply.com/sqfmi/watchy)
Now with injection molded or CNC'd aluminum case

Visit [**https://watchy.sqfmi.com**](https://watchy.sqfmi.com) for documentation, hardware design files, and more!

## Setup
1. In the Arduino IDE Boards Manager, install support for the ESP32. You can find instructions here: https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md
2. Install this library (search for **Watchy** in the library manager), and any other depdencies when prompted
3. Check out the examples under ```Examples``` -> ```Watchy```
4. Compile & Upload with these board settings:
    * Board: "ESP32 Dev Module"
    * Partition Scheme: "Minimal SPIFFS"
    * All Other Settings: leave to default

You may also have to install the [CP2104 USB to Serial drivers](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) if the port is not automatically detected

## Changelog
### 080821
- battery bar
- temperature app but there's a hardware issue
- watchface
    moved wifi and BLE icons
    DIN Black Font
- stopwatch (but doesn't allow deep sleep)
- [scrolling menu](https://gitlab.com/astory024/watchy/-/blob/master/src/Watchy.cpp) by Alex Story

### Unknown Date
- removed FWUpdate cos it's useless
    
### 210821
- added working NTP timesync (set to sync every 1 week)
    - seems like it works, am lazy to test but the time gets synced periodically so ain't complaining
- commneted out the default wifi config stuff
- tried out a new version of the stopwatch (still doesn't work)
    - stopwatch doesn't start
- darkmode toggle doesn't work
    - screen refreshes when pressing menu or down, but "darkmode" stays "on"

### 280821
- DIsabled step counter on the BMA423 (maybe save batt?)




## Future Work
- add dark mode setting to menu
    - in progress (probably some problem with variable scope)
- need to figure out how to handle button presses neatly eg for stopwatch
    - maybe multiple states under APP_STATE

- add low battery / battery saver mode
- callibrate time by changing ppm (?)


## Long term stuff
- design case (in progress actually)


# Nigel's personal Notes

## Fonts
DPI is 141
[Font converter](https://rop.nl/truetype2gfx/) for ttf fonts



## Quick notes
didn't update screens in the "fast menu" cases
    reordered the menu to put firmware n stuff last
Weather stuff commnented out to test
step counter commented out
temperature hardware problem
working on dark mode ATM