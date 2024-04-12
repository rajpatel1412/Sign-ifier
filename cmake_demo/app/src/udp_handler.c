#include "udp_handler.h"
#include "capture.h"

#include <assert.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_LEN 4096
#define PORT_T 3000
#define RPORT_T 1234
#define NUMBERS_PER_LINE 20
#define BUFFER_LENGTH 1024
#define HOST_ADDRESS "192.168.7.1"


// static bool socket_init = false;

static int socketDescriptorT;
static struct sockaddr_in sinT;
static struct sockaddr_in sinRemoteT;
static bool isShutdown = false;
static bool Socket_loopCondition;

static pthread_t udp_listenThread_id;
// static void sleepForMs(long long delayInMs);
// static void readFromFile(char *fileName, char *buff);
void *udp_listenThread();

typedef struct{
    char messageRx[MAX_LEN];
    char prevMessage[MAX_LEN];
    int bytesRx;
} MessageRx;

// typedef struct {
//     char messageTx[MAX_LEN];
//     bool first_command;
// } MessageTx;

static MessageRx command;
// static MessageTx reply;

// starts thread to listen for and send packets
void Socket_init(void)
{
    // assert(!socket_init);
    
    //Set port and IP
    memset(&sinT, 0, sizeof(sinT));
    sinT.sin_family = AF_INET;
    sinT.sin_addr.s_addr = htonl(INADDR_ANY);
    sinT.sin_port = htons(PORT_T);

    //Create and bind to socket
    socketDescriptorT = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptorT, (struct sockaddr*) &sinT, sizeof(sinT));

    sinRemoteT.sin_family = AF_INET;
    sinRemoteT.sin_port = htons(RPORT_T);
    sinRemoteT.sin_addr.s_addr = inet_addr(HOST_ADDRESS);

    Socket_loopCondition = true;
    // socket_init = true;
    pthread_create(&udp_listenThread_id, NULL, &udp_listenThread, NULL);
    
}
// gracefully exits the socket thread
void Socket_cleanup(void)
{
    // assert(socket_init);
    Socket_loopCondition = false;
    close(socketDescriptorT);
    pthread_join(udp_listenThread_id, NULL);
    // socket_init = false;
}

void Socket_listen(struct sockaddr_in *sinRemote)
{
    //Receive Data
    unsigned int sin_len = sizeof(*sinRemote);
    snprintf(command.prevMessage, MAX_LEN, command.messageRx);
    command.bytesRx = recvfrom(socketDescriptorT, command.messageRx, MAX_LEN -1, 0, (struct sockaddr *) sinRemote, &sin_len);
    command.messageRx[command.bytesRx] = 0; //Null terminated (string)
}

void Socket_reply()
{
    //Create reply
    if (strcmp(command.messageRx, "stop") == 0) {
        // exits the application
        isShutdown = true;
        Socket_loopCondition = false;
    }
    else {
        printf(command.messageRx);
        /*

        Add the function to display message on screen
        And do the text to speech conversion


        */
    }
}

int sendResponse(const void* str, int size) 
{
    int packetSent = 0;
    sendto(socketDescriptorT, str, size, 0, (struct sockaddr*) 
            &sinRemoteT, sizeof(sinRemoteT));
    return packetSent;
}

// thread that sends and recieves packets
void *udp_listenThread()
{
    struct sockaddr_in sinRemote;
    isShutdown = false;
    // reply.first_command = true;
    while (Socket_loopCondition) {
        Socket_listen(&sinRemote);
        Socket_reply(&sinRemote);
    }
    return NULL;
}

// to control shutdown
bool Socket_getIsShutdown(void)
{
    return isShutdown;
}

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

// Reads the value from a file and stores it in buff
// static void readFromFile(char *fileName, char *buff)
// {
//     FILE *pFile = fopen(fileName, "r");
//     if(pFile == NULL) {
//         printf("ERROR: Unable to open file %s for read\n", fileName);
//         exit(-1);
//     }

//     fgets(buff, BUFFER_LENGTH, pFile);
//     fclose(pFile);
// }