#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <connection.h>

#ifndef _server_h_
#define _server_h_

typedef struct TCPServer {
    char *serverListenAddress;
    unsigned short int serverListenPort;
    int listenSocketFd;
    char isSocketFdOpened;
    char isSocketBinded;
    char isSocketListening;

} TCPServer;

TCPServer *newTCPServer(unsigned short int port);
Connection *accept_wait(TCPServer *s);
int deleteTCPServer(TCPServer *s);

#endif /* _server_h_ */
