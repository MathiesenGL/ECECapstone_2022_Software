#pragma once

// This file serves as a wrapper for the Adafruit_GFX library to provide
// a more consistent interface.
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_HX8357.h>
#include <Adafruit_STMPE610.h>

#include "Vector2d.hpp"

//#define USE_ILI9341 1      // for 2.4" display


// Specify pins used to connect to display (this is ESP32-specific)
#define STMPE_CS  32
#define TFT_CS    15
#define TFT_DC    33
#define TFT_RST   -1
#define SD_CS     14

// This is calibration data for the raw touch data to the screen coordinates
// For rotation 1, these put the buttons at the top of the screen
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

// UI Buttondetails
#define BUTTON_X 100
#define BUTTON_Y 100
#define BUTTON_W 100
#define BUTTON_H 100
#define BUTTON_TEXTSIZE 5
#define DISPLAY_XOFFSET 80
#define DISPLAY_TEXTOFFSET 90
#define DISPLAY_YOFFSET 0

// standard color names
#define BLACK       0x0000
#define NAVY        0x000f
#define DARKGREEN   0x03e0
#define DARKCYAN    0x03ef
#define MAROON      0x7800
#define PURPLE      0x780f
#define OLIVE       0x7be0
#define LIGHTGREY   0xc618
#define DARKGREY    0x7bef
#define BLUE        0x001f
#define GREEN       0x07e0
#define CYAN        0x07ff
#define RED         0xf800
#define MAGENTA     0xf81f
#define YELLOW      0xffe0
#define WHITE       0xffff
#define ORANGE      0xfd20
#define GREENYELLOW 0xafe5
#define PINK        0xfc18

// Function prototypes
void graphicsInit();
void eraseScreen();
void fillScreen(uint16_t color = BLACK);
void drawPixel(const Vector2d& position, uint16_t color);
void drawLine(const Vector2d& start, const Vector2d& end, uint16_t color);
void drawRect(const Vector2d& ul, const Vector2d& size, uint16_t color);
void fillRect(const Vector2d& ul, const Vector2d& size, uint16_t color);
void drawCircle(const Vector2d& center, uint16_t radius, uint16_t color);
void fillCircle(const Vector2d& center, uint16_t radius, uint16_t color);
void drawRoundRect(const Vector2d& ul, const Vector2d& size, uint16_t r, uint16_t color);
void fillRoundRect(const Vector2d& ul, const Vector2d& size, uint16_t r, uint16_t color);
void drawChar(const Vector2d& position, char c, uint16_t color, uint16_t bg, uint8_t size);
const Vector2d screenSize();
bool initializeButtons(Adafruit_GFX_Button menuButtons[], char menuLabels[][2]);
int buttonPressed(Adafruit_GFX_Button menuButtons[], int buttonCount);
