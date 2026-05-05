# waveshare-ESP32-S3-2.8B-480x640-notouchTempSensorReadout-ST7701-LVGL9
Waveshare ESP32-S3 2.8B 480x640 using Display Driver ST7701 and LVGL9.5. Using Temp Sensor: DS18B20 sensor with 4.7k resistor added between IO pins.  
Simple coolant temp readout with no touch functions. Temp readout logic includes color changes to orange at 200°F and red at 210°F. 
This board does not support LCD rotation using ST7701.

The main .ino file and provided drivers are a good base to start a simple display in LVGL 9 using a ST7701 driver LCD. 

//Code Compatability: 
When purchasing this board (SKU: 30241	ESP32-S3-LCD-2.8B) from waveshare you can download additional demo files/drivers to add touch function, SD card, button functions here: https://docs.waveshare.com/ESP32-S3-LCD-2.8B . These files will be tailored to LVGL 8 UI creation. ***The demo code/files for display drivers and LVGL display from Waveshare will not work with the code provided in this project.***
I gutted and modified the original provided demo code to upgrade from LVGL 8 to LVGL 9. 
This build is working with LVGL 9.5 
The UI in this project uses a custom font added to increase font size substantially. (LVGL only provides up to 48px which struggles to fill a higher resolution screen)

//Board Notes:
This ESP32-S3-LCD 2.B 480x640 will not rotate the display to landscape mode with hardware. I was not able to rotate the display with LVGL either in software.
You could use this code as a base to upgrade another ST7701 driver board to LVGL 9. You would need to modify your current "Display_ST7701.h" and "Display_ST7701.cpp" files. 
Aside from little to no support from Waveshare the display looks great and the board has great IO connectivity. 

Physical Board Wiring: 
Plug the DS18B20 sensor into G, 3V3, and 16. 4.7k resistor across 3V3 and pin 16. I trashed the provided board with the DS18B20 I bought and installed the resistor instead. The pins do not align with this configuration. You might could use the usual provided DS18B20 input boards with G, 3V3, and TXT but I was not wanting to test this. Can cause issues with serial communication. 

Power by main USB-C port, or with 5 Volt power wired into "VCC and Ground" or "BAT and Ground" which has built in power regulation. Be careful with putting power into pins. You can fry your board quickly if using the wrong one. 


//Code UI: 
This code shows as portrait mode and has a coolant temp readout, a engine temp icon, and a Landcruiser icon up top (I designed this for my old Landcruiser). Replace the Landcruiser icon with another 480x wide icon generated with the Onlinge LVGL image converter tool.  
LVGL initializes the images separate from your standard create_UI. 
**You must save these images in lvgl/src/display/**

Setup Notes: 
*When configuring LVGL 9 dont put your conf.h file next to the main lvgl library folder. Put your conf.h file in /lvgl/src/. The #define LV_CONF_INCLUDE_SIMPLE included in the .ino will find it. 
*Remember to open your conf.h file and enable display. 



