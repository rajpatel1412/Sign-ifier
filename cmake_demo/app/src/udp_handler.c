// Source: adapted from Team Solar streaming Guide
// https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2022-student-howtos/StreamingWebcamFromBeagleBoneToNodeJSServer.pdf

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


// message information struct
typedef struct{
    char messageRx[MAX_LEN];
    int bytesRx;
} MessageRx;

static bool running = true; // loop control

static struct sockaddr_in sinT; // sending video
static struct sockaddr_in sinRemoteT; // sending video to website

//
static struct sockaddr_in sinJST; // sending/receiving commands to website
static struct sockaddr_in sinRemotePyT;  // streaming to python
static struct sockaddr_in sinRemotePy2T; // listening from python
static struct sockaddr_in sinRemoteJST; // sending/recieving commands to website
static MessageRx answer; // message from python
static MessageRx command; // message from node


pthread_t udpThreadID;
pthread_mutex_t displayMutex = PTHREAD_MUTEX_INITIALIZER;
static int socketDescriptorJST; // sending/recieving other data to website
//

static int socketDescriptorT; // sending video

bool udp_isRunning(void)
{
        return running;
}

//Initialize UDP connection
void openConnectionT() 
{       
        // socket streaming video to website and python
        memset(&sinT, 0, sizeof(sinT));
        sinT.sin_family = AF_INET;
        sinT.sin_addr.s_addr = htonl(INADDR_ANY);
        sinT.sin_port = htons(PORT_T);

        socketDescriptorT = socket(PF_INET, SOCK_DGRAM, 0);
        bind(socketDescriptorT, (struct sockaddr*) &sinT, sizeof(sinT));


        
        // socket communicating other data with js
        memset(&sinJST, 0, sizeof(sinJST));
        sinJST.sin_family = AF_INET;
        sinJST.sin_addr.s_addr = htonl(INADDR_ANY);
        sinJST.sin_port = htons(JS_PORT_T);

        socketDescriptorJST = socket(PF_INET, SOCK_DGRAM, 0);
        bind(socketDescriptorJST, (struct sockaddr*) &sinJST, sizeof(sinJST));
        


        // nodejs server video
        sinRemoteT.sin_family = AF_INET;
        sinRemoteT.sin_port = htons(RPORT_T);

        //serving from BBG - very slow
        // sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.2"); 

        // for serving from host
        sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.1");



        // nodejs server other commands
        sinRemoteJST.sin_family = AF_INET;
        sinRemoteJST.sin_port = htons(RPORT_T);

        //serving from BBG - very slow
        // sinRemoteJST.sin_addr.s_addr = inet_addr("192.168.7.2"); 

        // for serving from host
        sinRemoteJST.sin_addr.s_addr = inet_addr("192.168.7.1");



        // sending video to python
        sinRemotePyT.sin_family = AF_INET;        
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

//Send video frame using udp packet to nodejs
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

// Send video frame using udp packet to python
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

// Send other commands using udp packet to nodejs
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
        //Receive Data from python
        unsigned int sin_len = sizeof(sinRemotePy2T);
        answer.bytesRx = recvfrom(socketDescriptorT, answer.messageRx, MAX_LEN -1, 0, (struct sockaddr *) &sinRemotePy2T, &sin_len);
}

// recieve command from udp from nodejs
void getUdpCommands(void)
{
        unsigned int sin_len = sizeof(sinRemoteJST);
        command.bytesRx = recvfrom(socketDescriptorJST, command.messageRx, MAX_LEN -1, 0, (struct sockaddr *) &sinRemoteJST, &sin_len);
        printf("%s\n", command.messageRx);

        if(strcmp(command.messageRx, "infer") == 0) {
                        getAnswer();
                        pthread_mutex_lock(&displayMutex);
                                printf("%s\n", answer.messageRx);
                                lcd_display(answer.messageRx[0]);
                                sendResponseJST(returnDisplayString(), returnDisplayPos());
                        pthread_mutex_unlock(&displayMutex);     
                }
                
}  

//Close all sockets
void closeConnectionT()
{
        close(socketDescriptorT);
        close(socketDescriptorJST);
}

// thread controliing listening to nodejs
void* listenUDPThread(void* args)
{
        while(running) {
                // getAnswer();
                getUdpCommands();
        }  
        return args;
}

// start nodejs listening thread
void listenUDPThread_init(void)
{
        pthread_create(&udpThreadID, NULL, listenUDPThread, NULL);
}

// cleanup and clise the listeningf thread
void listenUDPThread_cleanup(void)
{
        running = false;
        pthread_join(udpThreadID, NULL);
}

void udp_setRUnning(bool value)
{
        running = value;
}

