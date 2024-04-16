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

static bool stopping = false;

static struct sockaddr_in sinT; // sending video
static struct sockaddr_in sinRemoteT; // sending video to website

//
static struct sockaddr_in sinJST; // sending commands to website
static struct sockaddr_in sinRemotePyT;  // streaming to python
static struct sockaddr_in sinRemotePy2T; // listening from python
static struct sockaddr_in sinRemoteJST; // sending commands to website
static MessageRx answer;
static MessageRx command;
bool loopCondition = true;
pthread_t udpThreadID;
static int socketDescriptorJST; // sending other data to website
//

static int socketDescriptorT; // sending video

bool udp_isStopping()
{
        return stopping;
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

        //serving from BBG - very slow
        // sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.2"); 

        // for serving from host
        sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.1");



        // nodejs server other commands
        sinRemoteJST.sin_family = AF_INET;
        sinRemoteJST.sin_port = htons(RPORT_T);

        //serving from BBG - very slow
        // sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.2"); 

        // for serving from host
        sinRemoteJST.sin_addr.s_addr = inet_addr("192.168.7.1");



        //  sedning to python
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
    answer.messageRx[answer.bytesRx] = '\0'; //Null terminated (string)
    printf("%s\n", answer.messageRx);
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
                sendResponseJST(answer.messageRx, answer.bytesRx);
        }  
        if(strcmp(command.messageRx, "play") == 0) {
                // play audio function
                (void) system("aplay test.wav");
        }
        if(strcmp(command.messageRx, "clear") == 0) {
                // clear text display
                answer.messageRx[0] = '\0';
                lcd_clear();
        }   
        if(strcmp(command.messageRx, "off") == 0) {
                // stop the whole system
                stopping = true;

        }
        // sendResponseJST(command.messageRx, command.bytesRx);
}

//Close udp connection
void closeConnectionT()
{
        close(socketDescriptorT);
}


void* listenThread(void* args)
{
        while(true) {
                getAnswer();
                getUdpCommands();
        }  
        // getAnswer();
        return args;
}

void listenThread_init(void)
{
        pthread_create(&udpThreadID, NULL, listenThread, NULL);
}

void listenThread_cleanup(void)
{
        loopCondition = false;
        pthread_join(udpThreadID, NULL);
}

