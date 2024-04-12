// UDP Network Manager

// Handles all things network related
// Listens to incoming UDP Packets on port 12345
// Treats each packet as a command to respond to
// Handles command to shutdown system

#include <netinet/in.h>
#ifndef _UDP_HANDLER_H_
#define _UDP_HANDLER_H_

#include <stdbool.h>

void Socket_init(void);
void Socket_cleanup(void);
void Socket_listen(struct sockaddr_in *sinRemote);
void Socket_reply();
bool Socket_getIsShutdown(void);
int sendResponse(const void* str, int size);

#endif