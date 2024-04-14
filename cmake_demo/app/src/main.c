#include "capture.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// Lets the program wait for the specified number of milliseconds
// For instance when waiting for the pins to be configured
// from assignment documentation
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

int main()
{
    capture_init();
    while(true) {}
    sleepForMs(20000);
    capture_cleanup();
}