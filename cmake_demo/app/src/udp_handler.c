#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

#include "udp_handler.h"
#include "hal/lcd.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <arpa/inet.h> 
#include <netdb.h> 
#include <pthread.h>


typedef struct{
    char messageRx[MAX_LEN];
    char prevMessage[MAX_LEN];
    int bytesRx;
} MessageRx;

static bool running = true;

static struct sockaddr_in sinT; // sending video
static struct sockaddr_in sinRemoteT; // sending video to website

//
static struct sockaddr_in sinJST; // sending commands to website
static struct sockaddr_in sinRemotePyT;  // streaming to python
static struct sockaddr_in sinRemotePy2T; // listening from python
static struct sockaddr_in sinRemoteJST; // sending commands to website
static MessageRx answer;
static MessageRx command;
pthread_t udpThreadID;
// pthread_t pythonThreadID;
pthread_mutex_t displayMutex = PTHREAD_MUTEX_INITIALIZER;
static int socketDescriptorJST; // sending other data to website
char displayString[1024];
int displayPosition = 0;
//

static int socketDescriptorT; // sending video

bool udp_isRunning(void)
{
        return running;
}

//Initialize UDP connection
void openConnectionT() 
{       
        // socket listening ffrom website
        memset(&sinT, 0, sizeof(sinT));
        sinT.sin_family = AF_INET;
        sinT.sin_addr.s_addr = htonl(INADDR_ANY);
        sinT.sin_port = htons(PORT_T);

        socketDescriptorT = socket(PF_INET, SOCK_DGRAM, 0);
        bind(socketDescriptorT, (struct sockaddr*) &sinT, sizeof(sinT));


        
        // socket communbicating other data with js
        memset(&sinJST, 0, sizeof(sinJST));
        sinJST.sin_family = AF_INET;
        sinJST.sin_addr.s_addr = htonl(INADDR_ANY);
        sinJST.sin_port = htons(JS_PORT_T);

        socketDescriptorJST = socket(PF_INET, SOCK_DGRAM, 0);
        bind(socketDescriptorJST, (struct sockaddr*) &sinJST, sizeof(sinJST));
        


        // nodejs server video
        sinRemoteT.sin_family = AF_INET;
        sinRemoteT.sin_port = htons(RPORT_T);

        //serving from BBG - very slow.7.1");



        // nodejs server other commands
        sinRemoteJST.sin_family = AF_INET;
        sinRemoteJST.sin_port = htons(RPORT_T);

        //serving from BBG - very slow
        // sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.2"); 
        sinRemotePyT.sin_port = htons(PYPORT_T);

        // serving from BBG - very slow
        // sinRemotePyT.sin_addr.s_addr = inet_addr("192.168.7.2"); 

        // for serving from host
        sinRemotePyT.sin_addr.s_addr = inet_addr("192.168.7.1");



        //  recieving from python
        sinRemotePy2T.sin_family = AF_INET;
        sinRemotePy2T.sin_port = htons(PORT_LISTEN_PY_T);

        // serving from BBG - very slow
        // sinRemotePy2T.sin_addr.s_addr = inet_addr("192.168.7.2"); 

        // for serving from host
        sinRemotePy2T.sin_addr.s_addr = inet_addr("192.168.7.1");
}

//Send video frame using udp packet
int sendResponseT(const void *str, int size) 
{
        int packetSent = 0;
        sendto(socketDescriptorT,
                        str,
                        size,
                        0,
                        (struct sockaddr *) &sinRemoteT, 
                        sizeof(sinRemoteT)
                  );        
        return packetSent;
}

// Send video frame using udp packet
int sendResponsePyT(const void *str, int size) 
{
        int packetSent = 0;
        sendto(socketDescriptorT,
                        str,
                        size,
                        0,
                        (struct sockaddr *) &sinRemotePyT, 
                        sizeof(sinRemotePyT)
                  );
       
        return packetSent;
}

// Send other commands using udp packet
int sendResponseJST(const void *str, int size) 
{
        int packetSent = 0;
        sendto(socketDescriptorJST,
                        str,
                        size,
                        0,
                        (struct sockaddr *) &sinRemoteJST, 
                        sizeof(sinRemoteJST)
                  );
       
        return packetSent;
}


// recieve inference from python
void getAnswer(void)
{
//         //Receive Data
//     unsigned int sin_len = sizeof(sinRemotePyT);
//     answer.bytesRx = recvfrom(socketDescriptorPyT, answer.messageRx, MAX_LEN -1, 0, (struct sockaddr *) &sinRemotePyT, &sin_len);
// //     answer.messageRx[answer.bytesRx] = 0; //Null terminated (string)
//     printf("%s\n", answer.messageRx);

         //Receive Data from python
    unsigned int sin_len = sizeof(sinRemotePy2T);
    answer.bytesRx = recvfrom(socketDescriptorT, answer.messageRx, MAX_LEN -1, 0, (struct sockaddr *) &sinRemotePy2T, &sin_len);
//     answer.messageRx[answer.bytesRx] = 0; //Null terminated (string)
//     printf("%s\n", answer.messageRx);
//     sendResponseJST(answer.messageRx, answer.bytesRx);

}

// recieve command from udp
void getUdpCommands(void)
{
        unsigned int sin_len = sizeof(sinRemoteJST);
        command.bytesRx = recvfrom(socketDescriptorJST, command.messageRx, MAX_LEN -1, 0, (struct sockaddr *) &sinRemoteJST, &sin_len);
        // answer.messageRx[answer.bytesRx] = 0; //Null terminated (string)
        printf("%s\n", command.messageRx);

        if(strcmp(command.messageRx, "inference") == 0) {
                // if (displayPosition < 16) {
                        getAnswer();
                        // if(strcmp(answer.messageRx, "") != 0) {
                        pthread_mutex_lock(&displayMutex);
                                displayString[displayPosition++] = answer.messageRx[0];
                                displayString[displayPosition] = '\0';
                                printf("%s\n", displayString);
                                sendResponseJST(displayString, displayPosition);
                                // lcd write message
                                lcd_display(answer.messageRx[0]);
                        pthread_mutex_unlock(&displayMutex);

                        // }
                        // else {
                                
                        // }
                        
                // }
                
        }  
        if(strcmp(command.messageRx, "play") == 0) {
                // play audio function

                // play audio
                displayString[0] = '\0';
                displayPosition = 0;
                // (void) system("aplay test.wav");
        }
        if(strcmp(command.messageRx, "clear") == 0) {
                // clear text display
                pthread_mutex_lock(&displayMutex);
                        // displayString[0] = '\0';
                        memset(displayString, 0, 1024 * sizeof(char));
                        displayPosition = 0;
                        lcd_clear();
                pthread_mutex_unlock(&displayMutex);
        }   
        if(strcmp(command.messageRx, "off") == 0) {
                // stop the whole system
                running = false;

        }
        // sendResponseJST(command.messageRx, command.bytesRx);
}

//Close udp connection
void closeConnectionT()
{
        close(socketDescriptorT);
}


void* listenUDPThread(void* args)
{
        while(running) {
                // getAnswer();
                getUdpCommands();
        }  
        // getAnswer();
        return args;
}

void listenUDPThread_init(void)
{
        pthread_create(&udpThreadID, NULL, listenUDPThread, NULL);
}

void listenUDPThread_cleanup(void)
{
        running = false;
        pthread_join(udpThreadID, NULL);
}

// void* listenPyThread(void* args)
// {
//         while(running) {
//                 getAnswer();
//                 // getUdpCommands();
//                 pthread_mutex_lock(&displayMutex);
//                 for(int i = 0; answer.messageRx[i] != '\0'; i++) {
//                         displayString[displayPosition++] = answer.messageRx[i];
//                         lcd_display(answer.messageRx[i]);        
//                 }
//                 displayString[displayPosition] = '\0';
//                 printf("%s\n", displayString);
//                 pthread_mutex_unlock(&displayMutex);
                
//         }  
//         // getAnswer();
//         return args;
// }

// void listenPyThread_init(void)
// {
//         pthread_create(&pythonThreadID, NULL, listenPyThread, NULL);
// }

// void listenPyThread_cleanup(void)
// {
//         running = false;
//         pthread_join(pythonThreadID, NULL);
// }

