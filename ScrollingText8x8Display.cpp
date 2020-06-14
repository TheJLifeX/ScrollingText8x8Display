#include "ScrollingText8x8Display.h"

// Constructor
ScrollingText8x8Display::ScrollingText8x8Display() {}

/**
 * Initialize the ScrollingText8x8Display.
 *
 * @param displayRowPinsIn - is an array that stores the arduino pin numbers of the corresponding row pins of the 8x8 LED display. see the picture below. See "matrix of the pin connections" table in the [Arduino Lesson - 8x8 LED display](https://osoyoo.com/2017/07/15/arduino-lesson-8x8-led-matrix) tutorial.
 * @param displayColumnPinsIn - `displayColumnPins` is an array that stores the arduino pin numbers of the corresponding column pins of the 8x8 LED display. See "matrix of the pin connections" table in the [Arduino Lesson - 8x8 LED display](https://osoyoo.com/2017/07/15/arduino-lesson-8x8-led-matrix) tutorial.
 * @param scrollingDirectionIn - Define the scrolling direction. Can be `TOP_TO_BOTTOM`, `RIGHT_TO_LEFT`, `BOTTOM_TO_TOP` or `LEFT_TO_RIGHT`. Default value is `RIGHT_TO_LEFT`.
 * @param characterOrientationIn - Define the character orientation. Can be `TOP`, `RIGHT`, `BOTTOM` or `LEFT`. `TOP` is the normal orientation of a character. So `RIGHT` means the character is rotated to right. Default value is `TOP`.
 */
void ScrollingText8x8Display::init(byte displayRowPinsIn[DISPLAY_SIZE], byte displayColumnPinsIn[DISPLAY_SIZE], ScrollingDirection scrollingDirectionIn, CharacterOrientation characterOrientationIn)
{
  memcpy(displayRowPins, displayRowPinsIn, sizeof(displayRowPins));
  memcpy(displayColumnPins, displayColumnPinsIn, sizeof(displayColumnPins));

  // initialize the output pins
  for (byte i = 0; i < DISPLAY_SIZE; i++)
  {
    pinMode(displayRowPins[i], OUTPUT);
    pinMode(displayColumnPins[i], OUTPUT);
  }

  scrollingDirection = scrollingDirectionIn;
  characterOrientation = characterOrientationIn;

  /*
   * Strategy to display a 8x8 character.
   *
   * Number of refresh: 30 or more (here 60) refreshes a second (so that the eye doesn’t detect any flickering)
   * we are doing a whole column in one frame, we only need to update at (60 * 8) Hz. 8 because of 8 column.
  */
  int frequenz = 60 * DISPLAY_SIZE;
  frameWaitTime = 1000.0 / frequenz; // 1 second divided through the frequenz.
  minimalDurationForOneCharacter = frameWaitTime * DISPLAY_SIZE;
}

/**
 * Set a character in the `circularBuffer` array.
 * A previous character in the buffer will be overwritten.
 */
void ScrollingText8x8Display::setCharacterInBuffer(byte newCharacter[DISPLAY_SIZE][DISPLAY_SIZE])
{
  // index is here the position where the previous character is stored. So we overwrite the
  // previous character with the new character.
  byte index = cursor < DISPLAY_SIZE ? 1 : 0;
  for (byte r = 0; r < DISPLAY_SIZE; r++)
  {
    for (byte c = 0; c < DISPLAY_SIZE; c++)
    {
      circularBuffer[r][c + (index * DISPLAY_SIZE)] = newCharacter[r][c];
    }
  }
}

/**
 * Display a text on a 8x8 LED display.
 *
 * @param text - The text to be displayed.
 * @param scrollingSpeed - The scrolling speed is between 1.0 (lowest speed) and 100.0 (highest speed). 90.0 is the default value.
 */
void ScrollingText8x8Display::displayText(String text, unsigned long scrollingSpeed)
{
  float duration = map(
      scrollingSpeed,
      MINIMAL_SCROLLING_SPEED,
      MAXIMAL_SCROLLING_SPEED,
      MAXIMAL_DURATION_FOR_ONE_CHARACTER,
      minimalDurationForOneCharacter);

  int textLength = text.length();
  for (int i = 0; i < textLength; i++)
  {
    int charAsInt = (int)(text.charAt(i));

    if (charAsInt < 0 || charAsInt >= NUMBER_OF_CHARACTERS)
    {
      charAsInt = UNKNOW_CHARACTER;
    }

    byte asciiCharacter[DISPLAY_SIZE];
    for (int k = 0; k < DISPLAY_SIZE; k++)
    {
      // `pgm_read_byte` function documentation, see: https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
      asciiCharacter[k] = pgm_read_byte(&(font_8x8[charAsInt][k]));
    }
    displayScrolling_8x8_AsciiCharacter(asciiCharacter, duration);
  }
}

/**
 * Display a scrolling text on a 8x8 display.
 *
 * @param asciiCharacter - Ascii character to be displayed.
 * @param duration - Define how long a 8x8 character will be displayed. The value is in milliseconds.
*/
void ScrollingText8x8Display::displayScrolling_8x8_AsciiCharacter(
    byte asciiCharacter[DISPLAY_SIZE],
    unsigned long duration)
{
  byte character[DISPLAY_SIZE][DISPLAY_SIZE];
  convertAsciiCharacterTo_8x8_Array(asciiCharacter, character);

  setCharacterInBuffer(character);

  byte numberOfIteration = (NUMBER_OF_CHARACTER_IN_BUFFER * DISPLAY_SIZE) - DISPLAY_SIZE;
  byte charactersToDiplayRowsLength = NUMBER_OF_CHARACTER_IN_BUFFER * DISPLAY_SIZE;
  for (byte i = 0; i < numberOfIteration; i++)
  {
    byte windowToDisplay[DISPLAY_SIZE][DISPLAY_SIZE];
    for (byte r = 0; r < DISPLAY_SIZE; r++)
    {
      byte count = 0;
      for (byte c = cursor % charactersToDiplayRowsLength; count < DISPLAY_SIZE; c = (c + 1) % charactersToDiplayRowsLength)
      {
        windowToDisplay[r][count] = circularBuffer[r][c];
        count++;
      }
    }

    switch (scrollingDirection)
    {
    case TOP_TO_BOTTOM:
      rotate90Clockwise(windowToDisplay);
      rotate90Clockwise(windowToDisplay);
      rotate90Clockwise(windowToDisplay);
      break;
    case BOTTOM_TO_TOP:
      rotate90Clockwise(windowToDisplay);
      break;
    case LEFT_TO_RIGHT:
      rotate90Clockwise(windowToDisplay);
      rotate90Clockwise(windowToDisplay);
      break;
    default:
      break;
    }

    display_8x8_Character(windowToDisplay, duration);
    cursor++;
  }

  // so the cursor doesnt go out of range
  cursor = cursor % charactersToDiplayRowsLength;
}

/**
 * Display a 8x8 Array on a 8x8 LED display.
 *
 * @param character - The 8x8 array (character) to be display. This array (character) contains in each cell eihter 0 or 1. The value 0 means LED off and 1 LED on.
 * @param duration - Define how long the 8x8 array will be displayed. Value is in milliseconds.
*/
void ScrollingText8x8Display::display_8x8_Character(byte character[DISPLAY_SIZE][DISPLAY_SIZE], unsigned long duration)
{
  float currentDuration = frameWaitTime;
  while (currentDuration < duration)
  {

    for (byte r = 0; r < DISPLAY_SIZE; r++)
    {
      clearDisplay();

      // display one frame, so one row.
      for (byte c = 0; c < DISPLAY_SIZE; c++)
      {
        byte shouldTurnLedOn = character[r][c];
        if (shouldTurnLedOn)
        {
          // a LED is ON only when a row is HIGH and the column LOW else OFF.
          digitalWrite(displayRowPins[r], HIGH);
          digitalWrite(displayColumnPins[c], LOW);
        }
      }
      delay(frameWaitTime);
      //

      currentDuration = currentDuration + frameWaitTime;
    }
  }
}

/**
 *  Clear display. So turn all LEDs off.
*/
void ScrollingText8x8Display::clearDisplay()
{
  for (byte i = 0; i < DISPLAY_SIZE; i++)
  {
    // a led is ON only when a row is HIGH and the column LOW else OFF.
    // so here we make all leds OFF.
    digitalWrite(displayRowPins[i], LOW);
    digitalWrite(displayColumnPins[i], HIGH);
  }
}

/*
 * Rotate a array 90 degree clockwise.
*/
void ScrollingText8x8Display::rotate90Clockwise(byte a[DISPLAY_SIZE][DISPLAY_SIZE])
{
  // Traverse each cycle
  for (byte i = 0; i < DISPLAY_SIZE / 2; i++)
  {
    for (byte j = i; j < DISPLAY_SIZE - i - 1; j++)
    {
      // Swap elements of each cycle
      // in clockwise direction
      byte temp = a[i][j];
      a[i][j] = a[DISPLAY_SIZE - 1 - j][i];
      a[DISPLAY_SIZE - 1 - j][i] = a[DISPLAY_SIZE - 1 - i][DISPLAY_SIZE - 1 - j];
      a[DISPLAY_SIZE - 1 - i][DISPLAY_SIZE - 1 - j] = a[j][DISPLAY_SIZE - 1 - i];
      a[j][DISPLAY_SIZE - 1 - i] = temp;
    }
  }
}

/**
 * @param asciiCharacter - ascii character with eight hexadecimal values.
 * @param character - The 8x8 array to store the converted ascii character.
 */
void ScrollingText8x8Display::convertAsciiCharacterTo_8x8_Array(byte asciiCharacter[DISPLAY_SIZE], byte character[DISPLAY_SIZE][DISPLAY_SIZE])
{
  for (byte k = 0; k < DISPLAY_SIZE; k++)
  {
    byte binary[DISPLAY_SIZE];
    byteToBinary(asciiCharacter[k], binary);

    for (byte t = 0; t < DISPLAY_SIZE; t++)
    {
      character[k][t] = binary[t];
    }
  }

  switch (characterOrientation)
  {
  case TOP:
    rotate90Clockwise(character);
    rotate90Clockwise(character);
    rotate90Clockwise(character);
    break;
  case BOTTOM:
    rotate90Clockwise(character);
    break;
  case LEFT:
    rotate90Clockwise(character);
    rotate90Clockwise(character);
    break;
  default:
    break;
  }
}

/*
 * Convert a signle byte to array with binary (0 or 1).
 * The binary array has a byte[] type because there isn't a binary type.
 * inspired from: https://stackoverflow.com/a/58053818/13480867
*/
void ScrollingText8x8Display::byteToBinary(byte byteIn, byte binaryOut[DISPLAY_SIZE])
{
  for (byte n = 0; n < DISPLAY_SIZE; n++)
  {
    binaryOut[7 - n] = (byteIn >> n) & 1;
  }
}
