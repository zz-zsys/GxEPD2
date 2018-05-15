// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#include "GxEPD2_583.h"
#include "WaveTables.h"

GxEPD2_583::GxEPD2_583(int8_t cs, int8_t dc, int8_t rst, int8_t busy) : GxEPD2_EPD(cs, dc, rst, busy, LOW, 20000000)
{
  _initial = true;
  _power_is_on = false;
  _using_partial_mode = false;
}

void GxEPD2_583::init(uint32_t serial_diag_bitrate)
{
  GxEPD2_EPD::init(serial_diag_bitrate);
  _initial = true;
  _power_is_on = false;
  _using_partial_mode = false;
}

void GxEPD2_583::clearScreen(uint8_t value)
{
  if (value == 0xFF) value = 0x33; // white value for this controller
  if (_initial)
  {
    _Init_Full();
    _writeCommand(0x10);
    for (uint32_t i = 0; i < WIDTH * HEIGHT / 2; i++)
    {
      _writeData(value);
    }
    _Update_Full();
    _initial = false;
  }
  if (!_using_partial_mode) _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x10);
  for (uint32_t i = 0; i < WIDTH * HEIGHT / 2; i++)
  {
    _writeData(value);
  }
  _Update_Part();
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x10);
  for (uint32_t i = 0; i < WIDTH * HEIGHT / 2; i++)
  {
    _writeData(value);
  }
  _Update_Part();
  _writeCommand(0x92); // partial out
  _initial = false;
}

void GxEPD2_583::writeScreenBuffer(uint8_t value)
{
  if (value == 0xFF) value = 0x33; // white value for this controller
  if (_initial) clearScreen(value);
  else _writeScreenBuffer(value);
}

void GxEPD2_583::_writeScreenBuffer(uint8_t value)
{
  if (!_using_partial_mode) _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x10);
  for (uint32_t i = 0; i < WIDTH * HEIGHT / 2; i++)
  {
    _writeData(value);
  }
  _writeCommand(0x92); // partial out
}

void GxEPD2_583::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  int16_t wb = (w + 7) / 8; // width bytes, bitmaps are padded
  x -= x % 8; // byte boundary
  w = wb * 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < WIDTH ? w : WIDTH - x; // limit
  int16_t h1 = y + h < HEIGHT ? h : HEIGHT - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;
  if (!_using_partial_mode) _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x10);
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data;
      // use wb, h of bitmap for index!
      int16_t idx = mirror_y ? j + dx / 8 + ((h - 1 - (i + dy))) * wb : j + dx / 8 + (i + dy) * wb;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&bitmap[idx]);
#else
        data = bitmap[idx];
#endif
      }
      else
      {
        data = bitmap[idx];
      }
      if (invert) data = ~data;
      _send8pixel(~data);
    }
  }
  _writeCommand(0x92); // partial out
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_583::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black)
  {
    writeImage(black, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_583::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    delay(1); // yield() to avoid WDT on ESP8266 and ESP32
    int16_t wb = (w + 1) / 2; // width bytes, bitmaps are padded
    x -= x % 2; // byte boundary
    w = wb * 2; // byte boundary
    int16_t x1 = x < 0 ? 0 : x; // limit
    int16_t y1 = y < 0 ? 0 : y; // limit
    int16_t w1 = x + w < WIDTH ? w : WIDTH - x; // limit
    int16_t h1 = y + h < HEIGHT ? h : HEIGHT - y; // limit
    int16_t dx = x1 - x;
    int16_t dy = y1 - y;
    w1 -= dx;
    h1 -= dy;
    if ((w1 <= 0) || (h1 <= 0)) return;
    if (!_using_partial_mode) _Init_Part();
    _writeCommand(0x91); // partial in
    _setPartialRamArea(x1, y1, w1, h1);
    _writeCommand(0x10);
    for (int16_t i = 0; i < h1; i++)
    {
      for (int16_t j = 0; j < w1 / 2; j++)
      {
        uint8_t data;
        // use wb, h of bitmap for index!
        int16_t idx = mirror_y ? j + dx / 2 + ((h - 1 - (i + dy))) * wb : j + dx / 2 + (i + dy) * wb;
        if (pgm)
        {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
          data = pgm_read_byte(&data1[idx]);
#else
          data = data1[idx];
#endif
        }
        else
        {
          data = data1[idx];
        }
        if (invert) data = ~data;
        _writeData(data);
      }
    }
    _writeCommand(0x92); // partial out
    delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  }
}

void GxEPD2_583::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_583::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_583::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_583::refresh(bool partial_update_mode)
{
  if (partial_update_mode) refresh(0, 0, WIDTH, HEIGHT);
  else
  {
    if (_using_partial_mode) _Init_Full();
    _Update_Full();
  }
}

void GxEPD2_583::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  x -= x % 8; // byte boundary
  w -= x % 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < WIDTH ? w : WIDTH - x; // limit
  int16_t h1 = y + h < HEIGHT ? h : HEIGHT - y; // limit
  w1 -= x1 - x;
  h1 -= y1 - y;
  if (!_using_partial_mode) _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _Update_Part();
  _writeCommand(0x92); // partial out
}

void GxEPD2_583::powerOff(void)
{
  _PowerOff();
}

void GxEPD2_583::_send8pixel(uint8_t data)
{
  for (uint8_t j = 0; j < 8; j++)
  {
    uint8_t t = data & 0x80 ? 0x00 : 0x03;
    t <<= 4;
    data <<= 1;
    j++;
    t |= data & 0x80 ? 0x00 : 0x03;
    data <<= 1;
    _writeData(t);
  }
}

void GxEPD2_583::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = (x + w - 1) | 0x0007; // byte boundary inclusive (last byte)
  uint16_t ye = y + h - 1;
  x &= 0xFFF8; // byte boundary
  _writeCommand(0x90); // partial window
  _writeData(x / 256);
  _writeData(x % 256);
  _writeData(xe / 256);
  _writeData(xe % 256);
  _writeData(y / 256);
  _writeData(y % 256);
  _writeData(ye / 256);
  _writeData(ye % 256);
  _writeData(0x01); // don't see any difference
  //_writeData(0x00); // don't see any difference
}

void GxEPD2_583::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_583::_PowerOff()
{
  _writeCommand(0x02); // power off
  _waitWhileBusy("_PowerOff", power_off_time);
  _power_is_on = false;
  _using_partial_mode = false;
}

void GxEPD2_583::_InitDisplay()
{
  if (!_power_is_on && (_rst >= 0))
  {
    digitalWrite(_rst, 0);
    delay(10);
    digitalWrite(_rst, 1);
    delay(10);
  }
  _writeCommand(0x01);
  _writeData (0x37); // POWER SETTING
  _writeData (0x00);
  _writeCommand(0X00); // PANNEL SETTING
  _writeData(0xCF);
  _writeData(0x08);
  _writeCommand(0x06); // boost
  _writeData (0xc7);
  _writeData (0xcc);
  _writeData (0x28);
  _writeCommand(0x30); // PLL setting
  //_writeData (0x39);   // PLL: 7s refresh
  _writeData (0x3a); // PLL: 15s refresh
  //_writeData (0x3c); // PLL: 30s refresh
  _writeCommand(0X41); // TEMPERATURE SETTING
  _writeData(0x00);
  _writeCommand(0X50); // VCOM AND DATA INTERVAL SETTING
  _writeData(0x77);
  _writeCommand(0X60); // TCON SETTING
  _writeData(0x22);
  _writeCommand(0x61); // 600*448
  _writeData (0x02);   // source 600
  _writeData (0x58);
  _writeData (0x01);   // gate 448
  _writeData (0xc0);
  _writeCommand(0X82); // VCOM VOLTAGE SETTING
  _writeData(0x28);    // all temperature  range
  _writeCommand(0xe5); // FLASH MODE
  _writeData(0x03);
}

void GxEPD2_583::_Init_Full()
{
  _InitDisplay();
  _PowerOn();
  _using_partial_mode = false;
}

void GxEPD2_583::_Init_Part()
{
  _InitDisplay();
  _PowerOn();
  _using_partial_mode = true;
}

void GxEPD2_583::_Update_Full()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Full", full_refresh_time);
}

void GxEPD2_583::_Update_Part()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Part", partial_refresh_time);
}


