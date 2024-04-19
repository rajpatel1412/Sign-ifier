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
pthread_mutex_t mutex;
bool lcd_loopCondition = true;
char signedLetter;
char message[1024];
int mpos;
bool isDisplaying;

char* returnDisplayString(void)
{
    return message;
}

int returnDisplayPos(void) 
{
    return mpos;
}

void *lcdThread(void *arg) {
    while(lcd_loopCondition) {
        pthread_mutex_lock(&mutex);
        if (isDisplaying) {
            writeChar(signedLetter);

        }
        isDisplaying = false;
        pthread_mutex_unlock(&mutex);
    }
    return arg;
}

void lcd_display(const char toDisplay)
{
    if (mpos > 1024) {
        printf("BUFFER OVERFLOW\n");
    }
    else {
        message[mpos++] = toDisplay;
        message[mpos] = '\0';
    }

    pthread_mutex_lock(&mutex);
    signedLetter = toDisplay;
    isDisplaying = true;
    pthread_mutex_unlock(&mutex);
    char command[4096];
    snprintf(command, 4096, "espeak \'%s\' -w test.wav", message);
    runCommand(command);
}

void lcd_clear()
{
    memset(message, 0, 1024 * sizeof(char));
    mpos = 0;

    // Set to command mode
    GPIO_writeValue(RS_GPIO_NUMBER, "0");

    // Clear the display.
	write4Bits(0x0); /* 0000 */
	write4Bits(0x1); /* 0001 */
	delayFor(0, 64000); // 64 us

	GPIO_writeValue(RS_GPIO_NUMBER, "1");
}

void initializeLCD()
{
    mpos = 0;

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
	write4Bits(0xC); /* 1111 */
	delayFor(0, 64000); // 64 us

    // printf("Completed initialization.\n");

	// Pull RS up to write data.
	GPIO_writeValue(RS_GPIO_NUMBER, "1");

    pthread_mutex_init(&mutex, NULL);
    pthread_create(&lcdThread_id, NULL, &lcdThread, NULL);
}

void lcd_cleanup(void) 
{
    GPIO_writeValue(RS_GPIO_NUMBER, "0");
   
    write4Bits(0x0);
	write4Bits(0x0);
	delayFor(0, 64000);

    GPIO_writeValue(RS_GPIO_NUMBER, "1");

    lcd_loopCondition = false;
    pthread_mutex_destroy(&mutex);
    pthread_join(lcdThread_id, NULL);
}

void writeMessage(char* msg)
{
    for (size_t i = 0; i < strlen(msg); i++) {
        writeChar(msg[i]);
    }
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
