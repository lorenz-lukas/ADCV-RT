#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <connection.h>

#ifndef _client_h_
#define _client_h_

typedef struct TCPClient {
    const char *ip;
    unsigned short int port;
    int clientSocketFd;
    char connected;
} TCPClient;

TCPClient *newTCPClient(const char *serverIp, unsigned short int serverPort);
Connection *connect_client(TCPClient *c);
int deleteTCPClient(TCPClient *c);

#endif /* _client_h_ */
