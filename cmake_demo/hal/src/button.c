#include "hal/button.h"
#include "../../app/include/udp_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "hal/gpio.h"
#include "hal/lcd.h"

static pthread_t buttonThread_id;
bool button_loopCondition = true;

void readFromFile(char file[], char buff[])
{
    FILE *pFile= fopen(file, "r");
    if (pFile == NULL)
    {
        printf("ERROR: Unable to open file (%s) for read\n", file);
        exit(1);
    }

    fgets(buff, LENGTH, pFile);
    
    fclose(pFile);
}

void *buttonThread(void *arg) {
    char values[3][2];
    while (button_loopCondition) {
        readFromFile(RED_VALUE, values[RED]);
        readFromFile(BLUE_VALUE, values[BLUE]);
        readFromFile(YELLOW_VALUE, values[YELLOW]);
        
        if (values[RED][0] == '1') {
            //SHUTDOWN
            udp_setRUnning(false);
            lcd_cleanup();
            sendResponseJST(returnDisplayString(), returnDisplayPos());
        }
        if (values[BLUE][0] == '1') {
            //CLEAR LCD CHARACTERS
            lcd_clear();
            sendResponseJST(returnDisplayString(), returnDisplayPos());
        }
        if (values[YELLOW][0] == '1') {
            //PLAY AUDIO
            runCommand("aplay test.wav");
        }
    }
    return arg;
}

void initializeButt(void) {
    GPIO_writeDirection(RED_GPIO_NUMBER, "in");
    GPIO_writeDirection(BLUE_GPIO_NUMBER, "in");
    GPIO_writeDirection(YELLOW_GPIO_NUMBER, "in");

    pthread_create(&buttonThread_id, NULL, &buttonThread, NULL);
}

void butt_cleanup(void) {
    button_loopCondition = false;
    pthread_join(buttonThread_id, NULL);
}
