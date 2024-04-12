#include "hal/lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "hal/gpio.h"

static pthread_t lcdThread_id;
bool lcd_loopCondition = true;
char message[1024];
bool isDisplaying;

void runCommand(char command[])
{
    FILE *pipe = popen(command, "r");
    char buffer[BUFFER_LENGTH];
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

static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

void *lcdThread(void *arg) {
    while(lcd_loopCondition) {
        if (isDisplaying) {
            char displayMessage[1024];
            strncpy(displayMessage, message, sizeof(displayMessage) - 1);
            displayMessage[sizeof(displayMessage) - 1] = '\0'; // Ensure null termination
            writeMessage(displayMessage);
        }
        isDisplaying = false;
    }
    return arg;
}

void lcd_display(const char* toDisplay)
{
    strncpy(message, toDisplay, sizeof(toDisplay) - 1);
    message[sizeof(message) - 1] = '\0';
    
    char command[4096];
    snprintf(command, 4096, "espeak \'%s\' -w test.wav", message);
    printf("command: %s", command);
    runCommand(command);
    sleepForMs(1000);
    runCommand("aplay test.wav");
    
    isDisplaying = true;
}

void lcd_clear()
{
    // Set to command mode
    GPIO_writeValue(RS_GPIO_NUMBER, "0");

    // Clear the display.
    write4Bits(0x0); /* 0000 */
    write4Bits(0x1); /* 0001 */
    delayFor(0, 64000); // 64 us

    // Sets mode to increment cursor position by 1 and shift right when writing to display.
    write4Bits(0x0); /* 0000 */
    write4Bits(0x6); /* 0110 */
    delayFor(0, 128000); // 128 us;

    // Pull RS up to write data.
    GPIO_writeValue(RS_GPIO_NUMBER, "1");
}

void initializeLCD()
{
    // Set every GPIO pin to OUTPUT
    GPIO_writeDirection(RS_GPIO_NUMBER, "out");
    GPIO_writeDirection(E_GPIO_NUMBER, "out");
    GPIO_writeDirection(D4_GPIO_NUMBER, "out");
    GPIO_writeDirection(D5_GPIO_NUMBER, "out");
    GPIO_writeDirection(D6_GPIO_NUMBER, "out");
    GPIO_writeDirection(D7_GPIO_NUMBER, "out");

    // Set every data pin to 0
    GPIO_writeValue(E_GPIO_NUMBER, "0");
    GPIO_writeValue(D4_GPIO_NUMBER, "0");
    GPIO_writeValue(D5_GPIO_NUMBER, "0");
    GPIO_writeValue(D6_GPIO_NUMBER, "0");
    GPIO_writeValue(D7_GPIO_NUMBER, "0");

    // printf("Configured pins...\n");

    // Set to command mode
    GPIO_writeValue(RS_GPIO_NUMBER, "0");

    // Special Function Set 1.
	write4Bits(0x03); // 0011
    delayFor(0, 5000000); // 5 ms

	// Special Function Set 2.
	write4Bits(0x03); // 0011
    delayFor(0, 128000); // 128 us
    
	// Special Function Set 3.
	write4Bits(0x03); // 0011
    delayFor(0, 128000); // 128 us

	// Sets to 4-bit operation.
	write4Bits(0x2); /* 0010 */
	delayFor(0, 1000000); // 1 ms

	// Sets to 4-bit operation. Sets 2-line display. Selects 5x8 dot character font.
	write4Bits(0x2); /* 0010 - We can alternatively write 0000 here for 8-bit operation. */
	write4Bits(0x8); /* 1000 - We can alternatively write 1100 here for 5x10 dot font. */
    delayFor(0, 128000); // 128 us

	// Display ON/OFF control.
	write4Bits(0x0); /* 0000 */
	write4Bits(0x8); /* 1000 */
	delayFor(0, 128000); // 128 us

    // Clear the display.
	write4Bits(0x0); /* 0000 */
	write4Bits(0x1); /* 0001 */
	delayFor(0, 64000); // 64 us

	// Sets mode to increment cursor position by 1 and shift right when writing to display.
	write4Bits(0x0); /* 0000 */
	write4Bits(0x6); /* 0110 */
	delayFor(0, 128000); // 128 us

	// Turns on display. This corresponds to the instruction 0000 1100 in binary.
	// To be able to see the cursor, use 0000 1110.
	// To enable cursor blinking, use 0000 1111.
	write4Bits(0x0); /* 0000 */
	write4Bits(0xF); /* 1111 */
	delayFor(0, 64000); // 64 us

    // printf("Completed initialization.\n");

	// Pull RS up to write data.
	GPIO_writeValue(RS_GPIO_NUMBER, "1");

    pthread_create(&lcdThread_id, NULL, &lcdThread, NULL);
}

void lcd_cleanup(void) 
{
    lcd_clear();
    lcd_loopCondition = false;
    pthread_join(lcdThread_id, NULL);
}

void writeMessage(char* msg)
{
    // printf("Writing \"%s\" to LCD...\n", msg);
    for (size_t i = 0; i < strlen(msg); i++) {
        writeChar(msg[i]);
    }
    // char command[4096];
    // snprintf(command, 4096, "espeak \'%s\' -w test.wav", msg);
    // printf("command: %s", command);
    // runCommand(command);
    // sleepForMs(1000);
    // runCommand("aplay test.wav");
}

void writeChar(char c)
{
	unsigned int upper_bits = (c >> 4);
	unsigned int lower_bits = c & 0xF;
	write4Bits(upper_bits);
	write4Bits(lower_bits);
}

void pulseEnable() {
    struct timespec pulseDelay = {0, 1000000};
    GPIO_writeValue(E_GPIO_NUMBER, "1");
    nanosleep(&pulseDelay, (struct timespec*) NULL);
    GPIO_writeValue(E_GPIO_NUMBER, "0");
    nanosleep(&pulseDelay, (struct timespec*) NULL);
}

void write4Bits(uint8_t value)
{
    char strBit[2];
    strBit[1] = '\0'; 
    strBit[0] = (value & 0x01 ? 1 : 0) + '0';
	GPIO_writeValue(D4_GPIO_NUMBER, strBit);

    strBit[0] = ((value >> 1) & 0x01 ? 1 : 0) + '0';
    GPIO_writeValue(D5_GPIO_NUMBER, strBit);

    strBit[0] = ((value >> 2) & 0x01 ? 1 : 0) + '0';
    GPIO_writeValue(D6_GPIO_NUMBER, strBit);

    strBit[0] = ((value >> 3) & 0x01 ? 1 : 0) + '0';
    GPIO_writeValue(D7_GPIO_NUMBER, strBit);
    
	pulseEnable();
}

void delayFor(int s, int ns)
{
    struct timespec delay = {s, ns};
    nanosleep(&delay, (struct timespec*) NULL);
}