#ifndef ScrollingText8x8Display_h
#define ScrollingText8x8Display_h

// include arduino
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "font_8x8.h"

#define DISPLAY_SIZE 8
#define UNKNOW_CHARACTER 95

#define MINIMAL_SCROLLING_SPEED 1.0
#define MAXIMAL_SCROLLING_SPEED 100.0
#define DEFAULT_SCROLLING_SPEED 90.0

// It is the maxiaml duration that used to display a character. Value is in milliseconds.
// This value is arbitrary set. So it can be greater.
#define MAXIMAL_DURATION_FOR_ONE_CHARACTER 1000

#define NUMBER_OF_CHARACTER_IN_BUFFER 2 // 2 because of previousCharacter and currentCharacter.

enum ScrollingDirection
{
  TOP_TO_BOTTOM,
  RIGHT_TO_LEFT,
  BOTTOM_TO_TOP,
  LEFT_TO_RIGHT
};

enum CharacterOrientation
{
  TOP,
  RIGHT,
  BOTTOM,
  LEFT
};

class ScrollingText8x8Display
{
public:
  ScrollingText8x8Display();
  void init(byte displayRowPinsIn[DISPLAY_SIZE],
            byte displayColumnPinsIn[DISPLAY_SIZE],
            ScrollingDirection scrollingDirection = RIGHT_TO_LEFT,
            CharacterOrientation characterOrientation = TOP);
  void displayText(String text, unsigned long scrollingSpeed = DEFAULT_SCROLLING_SPEED);

private:
  // Time to wait after one frame is displayed. In a frame a single row of LEDs (8 LEDs) are diplayed.
  float frameWaitTime;
  // It is the minimal duration that should be used to display a character. Value is in milliseconds.
  float minimalDurationForOneCharacter;
  // This array is a buffer used to display the scrolling text.
  byte circularBuffer[DISPLAY_SIZE][DISPLAY_SIZE * 2];
  byte cursor = 0;
  byte displayRowPins[DISPLAY_SIZE];
  byte displayColumnPins[DISPLAY_SIZE];
  ScrollingDirection scrollingDirection;
  CharacterOrientation characterOrientation;
  void setCharacterInBuffer(byte character[DISPLAY_SIZE][DISPLAY_SIZE]);
  void display_8x8_Character(byte character[DISPLAY_SIZE][DISPLAY_SIZE], unsigned long duration);
  void displayScrolling_8x8_AsciiCharacter(byte asciiCharacter[DISPLAY_SIZE], unsigned long duration);
  void clearDisplay();
  void rotate90Clockwise(byte a[DISPLAY_SIZE][DISPLAY_SIZE]);
  void convertAsciiCharacterTo_8x8_Array(byte asciiCharacter[DISPLAY_SIZE], byte character[DISPLAY_SIZE][DISPLAY_SIZE]);
  void byteToBinary(byte byteIn, byte binaryOut[DISPLAY_SIZE]);
};

#endif // ScrollingText8x8Display_h
