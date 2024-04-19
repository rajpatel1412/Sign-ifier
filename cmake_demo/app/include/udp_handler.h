// controlling the entire networking process between C, python and nodejs

#ifndef _UDP_HANDLER_H_
#define _UDP_HANDLER_H_

#include<stdbool.h>

#define PORT_T 3000 // video and website
#define JS_PORT_T 3001 // commands to node server
#define RPORT_T 1234 // video to node
#define PYPORT_T 12345 // video to python
#define PORT_LISTEN_PY_T 12346 // inferences from python


#define MAX_LEN 256

// control shutdown of entire operation
bool udp_isRunning(void);
void udp_setRUnning(bool value);

// initialize all the sockets
void openConnectionT();

// sending video to node and python
int sendResponseT(const void *str, int size);
int sendResponsePyT(const void *str, int size);

// send inference information to node
int sendResponseJST(const void *str, int size);

// receive inference from python
void getAnswer(void);

// close all sockets
void closeConnectionT(); 

// control the thread tha listens to node commands and respond to them
void listenUDPThread_init(void);
void listenUDPThread_cleanup(void);

// receive udp commands and respond to them.
void getUdpCommands(void);


#endif