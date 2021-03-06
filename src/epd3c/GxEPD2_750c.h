// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2_32

#ifndef _GxEPD2_750c_H_
#define _GxEPD2_750c_H_

#include "../GxEPD2_EPD.h"

class GxEPD2_750c : public GxEPD2_EPD
{
  public:
    // attributes
    static const uint16_t WIDTH = 640;
    static const uint16_t HEIGHT = 384;
    static const GxEPD2::Panel panel = GxEPD2::GDEW075Z09;
    static const bool hasColor = true;
    static const bool hasPartialUpdate = true;
    static const bool hasFastPartialUpdate = false;
    static const uint16_t power_on_time = 50; // ms, e.g. 36540us
    static const uint16_t power_off_time = 50; // ms, e.g. 40579us
    static const uint16_t full_refresh_time = 32000; // ms, e.g. 31094507us
    static const uint16_t partial_refresh_time = 32000; // ms, e.g. 31094507us
    // constructor
    GxEPD2_750c(int8_t cs, int8_t dc, int8_t rst, int8_t busy);
    // methods (virtual)
    void init(uint32_t serial_diag_bitrate = 0); // = 0 : disabled
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    void clearScreen(uint8_t value = 0x33); // init controller memory and screen (default white)
    void clearScreen(uint8_t black_value, uint8_t red_value); // init controller memory and screen
    void writeScreenBuffer(uint8_t value = 0x33); // init controller memory (default white)
    void writeScreenBuffer(uint8_t black_value, uint8_t red_value); // init controller memory
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    void writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImage(const uint8_t* black, const uint8_t* red, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write sprite of native data to controller memory, without screen refresh; x and w should be multiple of 8
    void writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    void drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void drawImage(const uint8_t* black, const uint8_t* red, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write sprite of native data to controller memory, with screen refresh; x and w should be multiple of 8
    void drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void refresh(bool partial_update_mode = false); // screen refresh from controller memory to full screen
    void refresh(int16_t x, int16_t y, int16_t w, int16_t h); // screen refresh from controller memory, partial screen
    void powerOff();
  private:
    void _writeScreenBuffer(uint8_t value);
    void _send8pixel(uint8_t black_data, uint8_t red_data);
    void _setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void _PowerOn();
    void _PowerOff();
    void _InitDisplay();
    void _Init_Full();
    void _Init_Part();
    void _Update_Full();
    void _Update_Part();
  protected:
    bool _initial, _power_is_on;
};

#endif

