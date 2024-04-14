#ifndef _UDP_HANDLER_H_
#define _UDP_HANDLER_H_

#define PORT_T 3000
#define RPORT_T 1234
#define PYPORT_T 12345


#define MAX_LEN 1024

void openConnectionT();
int sendResponseT(const void *str, int size);
int sendResponsePyT(const void *str, int size);
void getAnswer(void);
void closeConnectionT(); 

#endif