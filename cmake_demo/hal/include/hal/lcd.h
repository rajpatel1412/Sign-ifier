// Outputs string to LCD and plays it as audio
// Guide to get LCD working from
// https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2022-student-howtos/16x2CharacterLCDThroughGPIO.pdf
// Code referenced from
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
void initializeLCD();

void lcd_cleanup(void);
void lcd_display(const char* toDisplay);
void lcd_clear();

#endif