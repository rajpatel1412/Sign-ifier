#ifndef _UDP_HANDLER_H_
#define _UDP_HANDLER_H_

#include<stdbool.h>

#define PORT_T 3000
// #define PY_LISTEN_PORT_T 3000
#define JS_PORT_T 3001
#define RPORT_T 1234
#define PYPORT_T 12345
#define PORT_LISTEN_PY_T 12346


#define MAX_LEN 256

bool udp_isRunning(void);
void openConnectionT();
int sendResponseT(const void *str, int size);
int sendResponsePyT(const void *str, int size);
void getAnswer(void);
void closeConnectionT(); 
void listenUDPThread_init(void);
void listenUDPThread_cleanup(void);
void getUdpCommands(void);
// void listenPyThread_init(void);
// void listenPyThread_cleanup(void);

#endif