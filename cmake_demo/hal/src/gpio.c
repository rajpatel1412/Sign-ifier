/* GPIO Interface C Sample Code */
/* This code is intentionally left with room for improvement.
   Update this interface as needed. */
#include <stdio.h>
#include "hal/gpio.h"

#define D4_DIRECTION "/sys/class/gpio/gpio66/direction"
#define D4_VALUE "/sys/class/gpio/gpio66/value"

#define D5_DIRECTION "/sys/class/gpio/gpio69/direction"
#define D5_VALUE "/sys/class/gpio/gpio69/value"

#define D6_DIRECTION "/sys/class/gpio/gpio115/direction"
#define D6_VALUE "/sys/class/gpio/gpio115/value"

#define D7_DIRECTION "/sys/class/gpio/gpio48/direction"
#define D7_VALUE "/sys/class/gpio/gpio48/value"

#define RS_DIRECTION "/sys/class/gpio/gpio68/direction"
#define RS_VALUE "/sys/class/gpio/gpio68/value"

#define E_DIRECTION "/sys/class/gpio/gpio67/direction"
#define E_VALUE "/sys/class/gpio/gpio67/value"

static void writeToFile(const char* fileName, const char* value)
{
	FILE *pFile = fopen(fileName, "w");
	fprintf(pFile, "%s", value);
	fclose(pFile);
}

void GPIO_writeDirection(int gpio, char* dir)
{
    char* fileName;

    switch (gpio) {
        case D4_GPIO_NUMBER:
            fileName = D4_DIRECTION;
            break;
        case D5_GPIO_NUMBER:
            fileName = D5_DIRECTION;
            break;
        case D6_GPIO_NUMBER:
            fileName = D6_DIRECTION;
            break;
        case D7_GPIO_NUMBER:
            fileName = D7_DIRECTION;
            break;
        case RS_GPIO_NUMBER:
            fileName = RS_DIRECTION;
            break;
        case E_GPIO_NUMBER:
            fileName = E_DIRECTION;
            break;
    }
    writeToFile(fileName, dir);
}

void GPIO_writeValue(int gpio, char* val)
{
    char* fileName;

    switch (gpio) {
        case D4_GPIO_NUMBER:
            fileName = D4_VALUE;
            break;
        case D5_GPIO_NUMBER:
            fileName = D5_VALUE;
            break;
        case D6_GPIO_NUMBER:
            fileName = D6_VALUE;
            break;
        case D7_GPIO_NUMBER:
            fileName = D7_VALUE;
            break;
        case RS_GPIO_NUMBER:
            fileName = RS_VALUE;
            break;
        case E_GPIO_NUMBER:
            fileName = E_VALUE;
            break;
    }
    writeToFile(fileName, val);
}
