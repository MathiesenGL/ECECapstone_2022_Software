#include "Graphics.h"
#include "Vector2d.hpp"

// Definitions for 2.4" display only
#ifdef USE_ILI9341
#include <Adafruit_ILI9341.h>
static Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Definitions for 3.5" display only
#else
#include <Adafruit_HX8357.h>
static Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
#endif

Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// Initialize graphics system
void graphicsInit()
{
  // if the touchscreen returns true, assume we have the  tft device
  if (!ts.begin())
    return;

  tft.begin();
  fillScreen(BLACK);
}

// Fill screen with a color
void fillScreen(uint16_t color)
{
  tft.fillScreen(color);
}

void drawPixel(const Vector2d& position, uint16_t color)
{
  tft.drawPixel((uint16_t) position.getX(), (uint16_t) position.getY(), color);
}

void drawLine(const Vector2d& start, const Vector2d& end, uint16_t color)
{
  uint16_t x0 = start.getX();
  uint16_t y0 = start.getY();
  uint16_t x1 = end.getX();
  uint16_t y1 = end.getY();

  // check for fast lines
  if (x0 == x1) {
    if (y1 > y0) {
      tft.drawFastVLine(x0, y0, y1 - y0, color);
    }
    else {
      tft.drawFastVLine(x1, y1, y0 - y1, color);
    }
  }
  else if (y0 == y1) {
    if (x1 > x0) {
      tft.drawFastHLine(x0, y0, x1 - x0, color);
    }
    else {
      tft.drawFastHLine(x1, y1, x0 - x1, color);
    }
  }
  else {
    tft.drawLine( x0, y0, x1, y1, color);
  }
}

void drawRect(const Vector2d& ul, const Vector2d& size, uint16_t color)
{
  tft.drawRect( (uint16_t) ul.getX(), (uint16_t) ul.getY(), (uint16_t) size.getX(), (uint16_t) size.getY(), color);
}

void fillRect(const Vector2d& ul, const Vector2d& size, uint16_t color)
{
  tft.fillRect( (uint16_t) ul.getX(), (uint16_t) ul.getY(), (uint16_t) size.getX(), (uint16_t) size.getY(), color);

}

void drawCircle(const Vector2d& center, uint16_t radius, uint16_t color)
{
  tft.drawCircle( (uint16_t) center.getX(), (uint16_t) center.getY(), radius, color);
}

void fillCircle(const Vector2d& center, uint16_t radius, uint16_t color)
{
  tft.fillCircle( (uint16_t) center.getX(), (uint16_t) center.getY(), radius, color);
}

void drawRoundRect(const Vector2d& ul, const Vector2d& size, uint16_t r, uint16_t color)
{
  tft.drawRoundRect( (uint16_t) ul.getX(), (uint16_t) ul.getY(), (uint16_t) size.getX(), (uint16_t) size.getY(), r, color);

}
void fillRoundRect(const Vector2d& ul, const Vector2d& size, uint16_t r, uint16_t color)
{
  tft.fillRoundRect( (uint16_t) ul.getX(), (uint16_t) ul.getY(), (uint16_t) size.getX(), (uint16_t) size.getY(), r, color);

}

void drawChar(const Vector2d& position, char c, uint16_t color, uint16_t bg, uint8_t size)
{
  tft.drawChar( (uint16_t) position.getX(), (uint16_t) position.getY(), c, color, bg, size);
}

const Vector2d screenSize()
{
#ifdef USE_ILI9341
  //  return Vector2d(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT);
  return Vector2d(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT);
#else
  return Vector2d(HX8357_TFTWIDTH, HX8357_TFTHEIGHT);
#endif
}

bool initializeButtons(Adafruit_GFX_Button menuButtons[], char menuLabels[][2])
{
  menuButtons[0].initButtonUL(& tft,
                              0,
                              0,
                              BUTTON_W,
                              BUTTON_H,
                              BLACK,
                              BLACK,
                              WHITE,
                              menuLabels[0],
                              BUTTON_TEXTSIZE);
  menuButtons[0].drawButton();

  menuButtons[1].initButtonUL(& tft,
                              screenSize().getX() - BUTTON_W,
                              0,
                              BUTTON_W,
                              BUTTON_H,
                              BLACK,
                              BLACK,
                              WHITE,
                              menuLabels[1],
                              BUTTON_TEXTSIZE);
  menuButtons[1].drawButton();
  return true;
}
int buttonPressed(Adafruit_GFX_Button menuButtons[], int buttonCount)
{
  int btn = -1;
  TS_Point p;

  if (ts.bufferSize())
  {
    p = ts.getPoint();
  }
  else
  {
    // this is our way of tracking touch 'release'!
    p.x = p.y = p.z = -1;
  }

  // Scale from ~0->4000 to  tft.width using the calibration #'s
  if (p.z != -1)
  {
    int16_t px = p.x;
    int16_t py = p.y;
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  }

  // go thru all the buttons, checking if they were pressed
  for (int i = 0; i < buttonCount; i++)
  {
    if (menuButtons[i].contains(p.x, p.y))
    {
      menuButtons[i].press(true);  // tell the button it is pressed
    }
    else
    {
      menuButtons[i].press(false);  // tell the button it is NOT pressed
    }
  }

  // now we can ask the buttons if their state has changed
  for (int i = 0; i < buttonCount; i++)
  {
    if (menuButtons[i].justReleased())
    {
      btn = i;
    }
  }
  return btn;
}
