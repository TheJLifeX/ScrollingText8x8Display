#include "ScrollingText8x8Display.h"

ScrollingText8x8Display render;

void setup()
{
  byte displayRowPins[] = {2, 3, 4, 5, A3, A2, A1, A0}; // Ps: ``A3 = 17; A2 = 16; A1 = 15; A0 = 14;``
  byte displayColumnPins[] = {6, 7, 8, 9, 10, 11, 12, 13};

  ScrollingDirection scrollingDirection = RIGHT_TO_LEFT;
  CharacterOrientation characterOrientation = TOP;
  render.init(displayRowPins, displayColumnPins, scrollingDirection, characterOrientation);
}

void loop()
{
  render.displayText("Hello");
  render.displayText(" World!");
  render.displayText(" ");

  float scrollingSpeed = 55.5;
  render.displayText("slow.... text!", scrollingSpeed);
}
