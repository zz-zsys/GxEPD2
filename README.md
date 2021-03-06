# GxEPD2
## Arduino Display Library for SPI E-Paper Displays

- With full Graphics and Text support using Adafruit_GFX

- For SPI e-paper displays from Dalian Good Display 
- and SPI e-paper boards from Waveshare

### important note :
 - these displays are for 3.3V supply and 3.3V data lines.
 - never connect data lines directly to 5V data pins.

### Paged Drawing, Picture Loop
 - This library uses paged drawing to limit RAM use and cope with missing single pixel update support
 - buffer size can be selected in the application by template parameter page_height, see GxEPD2_Example
 - Paged drawing is implemented as picture loop, like in U8G2 (Oliver Kraus)
 - see https://github.com/olikraus/u8glib/wiki/tpictureloop
 - Paged drawing is also available using drawPaged() and drawCallback(), like in GxEPD
- ` // GxEPD style paged drawing; drawCallback() is called as many times as needed `
- ` void drawPaged(void (*drawCallback)(const void*), const void* pv) `
- paged drawing is done using Adafruit_GFX methods inside picture loop or drawCallback

### Full Screen Buffer Support
 - full screen buffer is selected by setting template parameter page_height to display height
 - drawing to full screen buffer is done using Adafruit_GFX methods without picture loop or drawCallback
 - and then calling method display()

### Low Level Bitmap Drawing Support
 - bitmap drawing support to the controller memory and screen is available:
 - either through the template class instance methods that forward calls to the base display class
 - or directy using an instance of a base display class and calling its methods directly

### Supporting Arduino Forum Topics:

- Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
- Good Dispay ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0

### Version 1.1.0
- added  support for GDEW0213I5F for 2.13" b/w 104x212 flexible display
- updated GxEPD2_WiFi_Example and GxEPD2_Spiffs_Loader to use BearSSL on ESP8266, for large bitmap downloads
#### Version 1.0.9
- add GxEPD2_U8G2_Fonts_Example, e.g. for use of Umlauts ÄÖÜäéöü
- NOTE: you need to SAVE the modified example to a saveable location for UTF-8 characters to work
#### Version 1.0.8
- add GxEPD2_SD_AVR_boards_added.h to GxEPD2_SD_AVR_Example
- with example definitions for non-AVR boards (low level display class use example)
#### Version 1.0.7
- add GxEPD2_boards_added.h to GxEPD2_Example 
- with example definitions for Arduino DUE and MKR1000
#### Version 1.0.6
- add buffered drawing option to GxEPD2_WiFi_Example
- allows use with 1.54" 3-color 200x200 display, may also be useful for small bitmaps
#### Version 1.0.5
- add buffered drawing option to GxEPD2_SD_Example and GxEPD2_Spiffs_Example
- allows use with 1.54" 3-color 200x200 display, may also be useful for small bitmaps
#### Version 1.0.4
- add GxEPD2_GFX base class support (optional, selectable, uses slightly more code)
- base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter
- add GxEPD2_GFX_MultiDisplayExample, uses  GxEPD2_GFX reference parameter
- modify GxEPD2_MultiDisplayExample for ESP8266 (template issue: class expected instead of typename)
#### Version 1.0.3
- fix GxEPD2_SD_Example & GxEPD2_SD_AVR_Example
- add GxEPD2_MultiDisplayExample (preliminary version)
#### Version 1.0.2
- initial release version 1.0.2
- tested with ESP8266, ESP32, STM32F103C8T6, AVR Arduino (Pro Mini 3.3V)
- 1.54" 3-color GxEPD2_154c can be used with paging for AVR
- wave tables in program space,  4.2" b/w can be used with GxEPD2_SD_AVR_Example
- issues enabled and welcome, please use Forum Topic for enhancement suggestions
#### Version 1.0.1
- pre-release test version
- GxEPD2_SD_AVR_Example added, has no graphics buffer to reduce RAM usage (base display class use)
- issues disabled so far, use Arduino Forum Topics instead
- in this version 1.54" 3-color GxEPD2_154c can only be used with full size buffer (or with no buffer base display class)
#### Version 1.0.0
- preliminary version, under construction
