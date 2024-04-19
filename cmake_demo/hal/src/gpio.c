/* GPIO Interface C Sample Code */
/* This code is intentionally left with room for improvement.
   Update this interface as needed. */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hal/gpio.h"

//LCD
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

//BUTTONS
#define RED_DIRECTION "/sys/class/gpio/gpio60/direction"
#define BLUE_DIRECTION "/sys/class/gpio/gpio30/direction"
#define YELLOW_DIRECTION "/sys/class/gpio/gpio31/direction"

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
        case RED_GPIO_NUMBER:
            fileName = RED_DIRECTION;
            break;
        case BLUE_GPIO_NUMBER:
            fileName = BLUE_DIRECTION;
            break;
        case YELLOW_GPIO_NUMBER:
            fileName = YELLOW_DIRECTION;
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

void runCommand(char command[])
{
    FILE *pipe = popen(command, "r");
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe))
    {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
    }

    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0)
    {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}
