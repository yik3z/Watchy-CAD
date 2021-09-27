# Watchy-CAD
Cases and hardware mods for Watchy

![Cases](Pictures/20210927_175318.jpg?raw=true)

# Cases
Cases are located under `CAD>Cases`. Each case design has the STLs under an `STLs` subfolder.

### *Slim* cases to make Watchy less chonky

1. **Slim**: This is the original case from [SQFMI's watchy-cases](https://github.com/sqfmi/watchy-cases). Had some errors slicing the file, hence rebuilt it as Slim V2 


2. **Slim V2**: Insipred by the `Slim` case, designed for use with 24mm NATO straps.


3. **Slim V3**: **Designed for a Watchy with the watch strap loops cut off from the PCB.** Similar to Slim V2, designed for 24mm NATO straps.
![SlimV4](Pictures/SlimV3/20210916_164747.jpg?raw=true)


4. **Slim V4**: Similar to V3 , but tabs are changed to fit the spring pin -style straps of the Watchy starter kit (24mm). This lowers the height of Watchy significantly. 
![SlimV4](Pictures/SlimV4/20210927_175651.jpg?raw=true)

## Some Warnings/Notes
**Slim V3 and Slim V4 don't have any fastening mechanisms in place; they are currently held together by friction, and a bit of double-sided tape.** I find that it works well (it's pretty difficult to take the PCB out of the case, but use at your own risk). I find that the double sided tape loses it's adhesion after sticking and removing it a few times, so it's a good idea to put a fresh piece of tape every few times you take it apart.


**The Slim V3 and V4 may or may not close fully if you have the stock battery cable. I shortened mine so there is less wire taking up space in the case. [See below](##-shorten-battery-wires) for details.** It allows the case to be 1mm thinner.  If you don't want to cut the battery wires, you can just make the case 1mm deeper :P

These cases were designed on Autodesk Inventor. The raw files are included in case you'd like to modify them. 
Also, tolerances may vary based on 3d printer.


  
## Upcoming Stuff
- Clipping mechanism to make the case more secure


# Hardware Mods


## Cutting off PCB loops

![cut tabs](Pictures/cut_tabs.jpg?raw=true)

(Guide may be coming soon)


## Shorten Battery Wires
1. Cut the wires.
2. Cut off excess.
3. Slip on some heatshrink.
4. Solder the wires back.
5. Heatshrink.

It's a good idea to do each wire separately so there's no chance that the + and - wires will ever touch.
![shortened wire](Pictures/20210927_182230.jpg?raw=true)

## Touchscreen
(coming soon too hopefully hehe)
The plan: integrate [this touchscreen](https://www.good-display.com/product/258.html) with Watchy. Is currently shipping. Will update progress soon.


## Battery Charging indicator
Idea is to connect the charging controller to the ESP32's GPIO and set an interrupt so it knows when it's charging.
*Fundamentally this is problematic - Watchy may never be able to wake from sleep upon connecting a charger.*

Explanation: the charging IC pulls pin 1 low when charger is connected. ESP32 can only have one GPIO pin configured (at a time) to wake it from sleep when the pin goes low (using `ext0`). Currently that is connected to the RTC (removing that means Watchy stops being a watch). An alternative is to use an inverter, but...that's a lot of work. So this mod may not happen. At least not the wake from deep sleep part

