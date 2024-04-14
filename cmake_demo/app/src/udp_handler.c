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

typedef struct{
    char messageRx[MAX_LEN];
    char prevMessage[MAX_LEN];
    int bytesRx;
} MessageRx;

static struct sockaddr_in sinT;
static struct sockaddr_in sinRemoteT;

//
static struct sockaddr_in sinRemotePyT;
static MessageRx answer;
//

static int socketDescriptorT;

//Initialize UDP connection
void openConnectionT() 
{
        memset(&sinT, 0, sizeof(sinT));
        sinT.sin_family = AF_INET;
        sinT.sin_addr.s_addr = htonl(INADDR_ANY);
        sinT.sin_port = htons(PORT_T);

        socketDescriptorT = socket(PF_INET, SOCK_DGRAM, 0);
        bind(socketDescriptorT, (struct sockaddr*) &sinT, sizeof(sinT));
        
        // nodejs server
        sinRemoteT.sin_family = AF_INET;
        sinRemoteT.sin_port = htons(RPORT_T);

        //serving from BBG - very slow
        // sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.2"); 

        // for serving from host
        sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.1");

        // python
        sinRemotePyT.sin_family = AF_INET;
        sinRemotePyT.sin_port = htons(PYPORT_T);

        // serving from BBG - very slow
        // sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.2"); 

        // for serving from host
        sinRemotePyT.sin_addr.s_addr = inet_addr("192.168.7.1");
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
        //Receive Data
    unsigned int sin_len = sizeof(sinRemotePyT);
    answer.bytesRx = recvfrom(socketDescriptorT, answer.messageRx, MAX_LEN -1, 0, (struct sockaddr *) &sinRemotePyT, &sin_len);
    answer.messageRx[answer.bytesRx] = 0; //Null terminated (string)
    printf("%s\n", answer.messageRx);
}

//Close udp connection
void closeConnectionT() 
{
        close(socketDescriptorT);
}




// static struct sockaddr_in sinPyT;
// static struct sockaddr_in sinRemotePyT;
// static int socketDescriptorT;

// //Initialize UDP connection
// void openConnectionT() 
// {
//         memset(&sinT, 0, sizeof(sinT));
//         sinT.sin_family = AF_INET;
//         sinT.sin_addr.s_addr = htonl(INADDR_ANY);
//         sinT.sin_port = htons(PORT_T);

//         socketDescriptorT = socket(PF_INET, SOCK_DGRAM, 0);
//         bind(socketDescriptorT, (struct sockaddr*) &sinT, sizeof(sinT));
        
//         sinRemoteT.sin_family = AF_INET;
//         sinRemoteT.sin_port = htons(RPORT_T);
//         sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.1");
// }

// //Send video frame using udp packet
// int sendResponseT(const void *str, int size) 
// {
//         int packetSent = 0;
//         sendto(socketDescriptorT,
//                         str,
//                         size,
//                         0,
//                         (struct sockaddr *) &sinRemoteT, 
//                         sizeof(sinRemoteT)
//                   );

                
//         return packetSent;
// }

// //Close udp connection
// void closeConnectionT() 
// {
//         close(socketDescriptorT);
// }