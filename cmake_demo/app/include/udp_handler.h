#ifndef _UDP_HANDLER_H_
#define _UDP_HANDLER_H_

#define PORT_T 3000
#define RPORT_T 1234

void openConnectionT();
int sendResponseT(const void *str, int size);
void closeConnectionT(); 

#endif