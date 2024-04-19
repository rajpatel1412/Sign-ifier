// Outputs characters to LCD, appends them into a string and plays it as audio
// Guide to get LCD working from
// How-To 16x2 Character LCD through GPIO (Spring 2022)
// https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2022-student-howtos/16x2CharacterLCDThroughGPIO.pdf

// Code referenced from How-To 16x2 Character LCD through GPIO (Spring 2022) support files
// https://opencoursehub.cs.sfu.ca/bfraser/solutions/433/StudentHowToGuides/2022Files/16x2CharacterLCDThroughGPIO.zip
// https://github.com/arduino-libraries/LiquidCrystal/blob/master/src/LiquidCrystal.cpp

#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>
#define BUFFER_LENGTH 1024

// nanosleep wrapper function - accepts seconds and nanoseconds to construct delay
void delayFor(int, int);
// Flash the E pin high to low to have the LCD consume the data
void pulseEnable();
// Write 4 bits to their corresponding pin (D4, D5, D6, D7)
void write4Bits(uint8_t);
// Write a char to the LCD
void writeChar(char);
// Write a message to the LCD
void writeMessage(char* msg);
// Initialize the LCD
void initializeLCD(void);
// Returns string to be displayed on website
char* returnDisplayString(void);
// Returns position of where char value was last appended
int returnDisplayPos(void);
// Cleans up LCD
void lcd_cleanup(void);
// Writes character to screen and creates a .wav file for text-to-speech
void lcd_display(const char toDisplay);
// Clears text on LCD
void lcd_clear(void);

#endif
