# waveshare-ESP32-S3-2.8B-480x640-notouchTempSensorReadout-ST7701-LVGL9
Waveshare ESP32-S3 2.8B 480x640 using Display Driver ST7701 and LVGL9.5. Using Temp Sensor: DS18B20 sensor with 4.7k resistor added between IO pins.  
Simple coolant temp readout with no touch functions. Temp readout logic includes color changes to orange at 200°F and red at 210°F. 
This board does not support LCD rotation using ST7701.

**The main .ino file and provided drivers are a good base to start a simple display in LVGL 9 using a ST7701 driver LCD**
**Aside from the "lv_conf.h" file and the "tempfont200.c" file all provided files will go in sketch folder alongside the TempSensor.ino file.**

//**Code Compatability**// 
When purchasing this board (SKU: 30241	ESP32-S3-LCD-2.8B) from waveshare you can download additional demo files/drivers to add touch function, SD card, button functions here: https://docs.waveshare.com/ESP32-S3-LCD-2.8B . These files will be very tailored to LVGL 8 UI creation. ***The demo code/files for display drivers and LVGL display from Waveshare will not work with the code provided in this project.***
I gutted and modified the original provided demo code to upgrade from LVGL 8 to LVGL 9. My motivation for upgrading to LVGL 9 was to get larger font sizes and other functionality down the road. I could not get larger fonts to work in LVGL 8 with the online font converter tool. https://lvgl.io/tools/fontconverter
This build is working with LVGL 9.5

//**Board Notes**//
This ESP32-S3-LCD 2.B 480x640 will not rotate the display to landscape mode with hardware. I was not able to rotate the display with LVGL either in software.
You could use this code as a base to upgrade another ST7701 driver board to LVGL 9. You would need to modify your current "Display_ST7701.h" and "Display_ST7701.cpp" files. 
Aside from little to no support from Waveshare, the display looks great and the board has great IO connectivity. 

//**Physical Board Wiring**// 
Plug the DS18B20 sensor into G, 3V3, and 16. 4.7k resistor across 3V3 and pin 16. I trashed the provided board with the DS18B20 I bought and installed the resistor instead. The esp32-s3 pins do not align with the sensor board configuration and the height was too large for my enclosure. You might use the usual provided DS18B20 input boards with G, 3V3, and TXT (aligned pins) but I was not wanting to test this. Can cause issues with serial communication. 

Power by main USB-C port, or with 5 Volt power wired into "VCC and Ground" or "BAT and Ground" which has built in power regulation. Be careful with putting power into pins. You can fry your board quickly if using the wrong one. 

//**UI Notes**// 
This code shows as portrait mode and has a coolant temp readout, a engine temp icon, and a Landcruiser icon up top (I designed this for my old FJ60 Landcruiser). Replace the Landcruiser icon with another 480x wide icon generated with the Online LVGL image converter tool. https://lvgl.io/tools/imageconverter  
LVGL initializes the images separate from your standard create_UI. 
The UI in this project uses a custom font added to increase font size substantially. (LVGL only provides up to 48px which struggles to fill a higher resolution screen)
**You must save the provided tempfont200.c file in lvgl/src/display/** or replace with another font and adjust the font declare code in the .ino 
*///Custom Fonts stored in src/display
//LV_FONT_DECLARE(tempfont200);
//#define font200 &tempfont200*
replace tempfont200 with the name of your custom font (tip: When using the font converter I went with 1 bit per pixel and include symbols: -.0123456789°FC )


//**Setup Notes**//
*When configuring LVGL 9 dont put your conf.h file next to the main lvgl library folder. Put your lv_conf.h file in /lvgl/src/. The #define LV_CONF_INCLUDE_SIMPLE included in the .ino will find it. 
*You will need OneWire and DallasTemperature libraries downloaded in addition to the LVGL 9 library. 
*Remember to open your lv_conf.h file and ensure you enable display at the top. 
*Ensure in you lv_conf.h file you have &lv_font_montserrat_48 enabled. Change "0" to "1". 



