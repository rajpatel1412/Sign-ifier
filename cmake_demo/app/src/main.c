#include "capture.h"
#include "hal/lcd.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

// // Lets the program wait for the specified number of milliseconds
// // For instance when waiting for the pins to be configured
// // from assignment documentation
// static void sleepForMs(long long delayInMs)
// {
//     const long long NS_PER_MS = 1000 * 1000;
//     const long long NS_PER_SECOND = 1000000000;
//     long long delayNs = delayInMs * NS_PER_MS;
//     int seconds = delayNs / NS_PER_SECOND;
//     int nanoseconds = delayNs % NS_PER_SECOND;
//     struct timespec reqDelay = {seconds, nanoseconds};
//     nanosleep(&reqDelay, (struct timespec *) NULL);
// }

int main(int argc, char **argv)
{
        initializeLCD();
        captureThread_init(argc, argv);
        // sleepForMs(10000);
        // printf("/n/n---------------------------Ending Sleep-----------------------------/n/n");
        bool shutdown = false;
        while(true) {
                if(shutdown) {
                        captureThread_cleanup();
                        lcd_cleanup();
                }
        }
        
        return 0;
}