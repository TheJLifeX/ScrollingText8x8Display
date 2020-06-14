# ScrollingText8x8Display

This library allows you to display a scrolling text on a 8x8 LED display.

You can set the scrolling speed, the scrolling direction and also the character orientation.

[Download it from GitHub][1].


## Example

### Hardware

Read the tutorials below to learn to setup a 8x8 LED display with arduino:
* [Arduino Lesson - 8x8 LED display][2]
* [Use 8x8 LED display to display letters.][3]

### Software

To install, copy the ScrollingText8x8Display folder into your arduino sketchbook\-libraries folder. More detailed instructions are [here][4].
The Library Manager can be used from arduino version 1.6.2.


### Setting up

```c++
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
```


## Documentation


### Initialize the ScrollingText8x8Display.

 * @param `displayRowPins` - is an array that stores the arduino pin numbers of the corresponding row pins of the 8x8 LED display. see the picture below. See "matrix of the pin connections" table in the [Arduino Lesson - 8x8 LED display][2] tutorial.
 * @param `displayColumnPins` - is an array that stores the arduino pin numbers of the corresponding column pins of the 8x8 LED display. See "matrix of the pin connections" table in the [Arduino Lesson - 8x8 LED display][2] tutorial.
 * @param `scrollingDirection` - optional - Define the scrolling direction. Can be `TOP_TO_BOTTOM`, `RIGHT_TO_LEFT`, `BOTTOM_TO_TOP` or `LEFT_TO_RIGHT`. Default value is `RIGHT_TO_LEFT`.
 * @param `characterOrientation` - optional - Define the character orientation. Can be `TOP`, `RIGHT`, `BOTTOM` or `LEFT`. `TOP` is the normal orientation of a character. So `RIGHT` means the character is rotated to right. Default value is `TOP`.

```c++
void init(byte displayRowPins[], byte displayColumnPins[], ScrollingDirection scrollingDirection, CharacterOrientation characterOrientation);
```
___

### Display a text on a 8x8 LED display.
* @param `text` - The text to be displayed.
* @param `scrollingSpeed` - optional - The scrolling speed is between 1.0 (lowest speed) and 100.0 (highest speed). 90.0 is the default value.
```c++
  void displayText(String text, unsigned long scrollingSpeed);
```

---

### ScrollingDirection enumeration
```c++
enum ScrollingDirection
{
  TOP_TO_BOTTOM,
  RIGHT_TO_LEFT,
  BOTTOM_TO_TOP,
  LEFT_TO_RIGHT
};
```

---

### CharacterOrientation enumeration
```c++
enum CharacterOrientation
{
  TOP,
  RIGHT,
  BOTTOM,
  LEFT
};
```



[1]: https://github.com/TheJLifeX/ScrollingText8x8Display/archive/master.zip
[2]: https://osoyoo.com/2017/07/15/arduino-lesson-8x8-led-matrix
[3]: https://mega.nz/folder/TI1QgAKQ#DpCOElh-b6mnEuqUMAVcqQ/folder/aAlUDCwZ
[4]: http://arduino.cc/en/Guide/Libraries
