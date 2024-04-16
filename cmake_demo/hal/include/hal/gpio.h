#ifndef _GPIO_H_
#define _GPIO_H_

#define LENGTH 2

//LCD
#define D4_GPIO_NUMBER 66
#define D5_GPIO_NUMBER 69
#define D6_GPIO_NUMBER 115
#define D7_GPIO_NUMBER 48
#define RS_GPIO_NUMBER 68
#define E_GPIO_NUMBER  67

//BUTTONS
#define RED 0
#define BLUE 1
#define YELLOW 2

#define RED_NUMBER 60
#define BLUE_NUMBER 30
#define YELLOW_NUMBER 31

#define RED_VALUE "/sys/class/gpio/gpio60/value"
#define BLUE_VALUE "/sys/class/gpio/gpio30/value"
#define YELLOW_VALUE "/sys/class/gpio/gpio31/value"

// Write the provided string "in"/"out" to the given GPIO's direction
void GPIO_writeDirection(int, char*);
// Write the provided string "1"/"0" to the given GPIO's value
void GPIO_writeValue(int, char*);

void runCommand(char command[]);

void sleepForMs(long long delayInMs);

#endif
