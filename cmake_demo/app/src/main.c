#include "capture.h"

#include "hal/lcd.h"
#include "hal/button.h"
#include <time.h>
#include "udp_handler.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


int main()
{       
    // initialize     
    capture_init();
    listenUDPThread_init();
    initializeLCD();
    initializeButt();

    // wait till stop is called
    while(udp_isRunning()) {}

    // cleanup all devices
    butt_cleanup();
    lcd_cleanup();
    listenUDPThread_cleanup();
    capture_cleanup();

    return 0;

}