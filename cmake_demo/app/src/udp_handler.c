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

static struct sockaddr_in sinT;
static struct sockaddr_in sinRemoteT;

//
// static struct sockaddr_in sinPyT;
static struct sockaddr_in sinRemotePyT;  // streaming to python
static struct sockaddr_in sinRemotePy2T; // listening from python
static MessageRx answer;
bool loopCondition = true;
pthread_t udpThreadID;
// static int socketDescriptorPyT;
//

static int socketDescriptorT;

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
        
        // socket listening ot python
        // memset(&sinPyT, 0, sizeof(sinPyT));
        // sinPyT.sin_family = AF_INET;
        // sinPyT.sin_addr.s_addr = htonl(INADDR_ANY);
        // sinPyT.sin_port = htons(PY_LISTEN_PORT_T);

        // socketDescriptorPyT = socket(PF_INET, SOCK_DGRAM, 0);
        // bind(socketDescriptorPyT, (struct sockaddr*) &sinPyT, sizeof(sinPyT));
        
        // nodejs server
        sinRemoteT.sin_family = AF_INET;
        sinRemoteT.sin_port = htons(RPORT_T);

        //serving from BBG - very slow
        // sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.2"); 

        // for serving from host
        sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.1");

        //  sedning to python
        sinRemotePyT.sin_family = AF_INET;
        sinRemotePyT.sin_port = htons(PYPORT_T);

        // serving from BBG - very slow
        // sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.2"); 

        // for serving from host
        sinRemotePyT.sin_addr.s_addr = inet_addr("192.168.7.1");



        //  recieving from python
        sinRemotePy2T.sin_family = AF_INET;
        sinRemotePy2T.sin_port = htons(PORT_LISTEN_PY_T);

        // serving from BBG - very slow
        // sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.2"); 

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

void getAnswer(void)
{
//         //Receive Data
//     unsigned int sin_len = sizeof(sinRemotePyT);
//     answer.bytesRx = recvfrom(socketDescriptorPyT, answer.messageRx, MAX_LEN -1, 0, (struct sockaddr *) &sinRemotePyT, &sin_len);
// //     answer.messageRx[answer.bytesRx] = 0; //Null terminated (string)
//     printf("%s\n", answer.messageRx);

         //Receive Data
    unsigned int sin_len = sizeof(sinRemotePy2T);
    answer.bytesRx = recvfrom(socketDescriptorT, answer.messageRx, MAX_LEN -1, 0, (struct sockaddr *) &sinRemotePy2T, &sin_len);
    answer.messageRx[answer.bytesRx] = '\0'; //Null terminated (string)
    printf("%s\n", answer.messageRx);
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