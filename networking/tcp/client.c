#include <client.h>

#ifndef _client_c_
#define _client_c_

TCPClient *newTCPClient(const char *serverIp, unsigned short int serverPort) {
    TCPClient *c = NULL;
    c = (TCPClient*)malloc(sizeof(TCPClient));
    if (c == NULL) {
        fprintf(stderr, "%s\n", strerror(errno));
        return NULL;
    }
    memset(c, 0, sizeof(TCPClient));
    c->clientSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (c->clientSocketFd == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        free(c);
        return NULL;
    }
    c->ip = serverIp;
    c->port = serverPort;
    return c;
}

Connection *connect_client(TCPClient *c) {
    if (c == NULL) {
        fprintf(stderr, "NULL Connection instance.\n");
        return NULL;
    }
    if (c->connected) {
        fprintf(stderr, "Instance already connected.\n");
        return NULL;
    }
    struct sockaddr_in serverAddress;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    memset(&serverAddress, 0, sizeof(struct sockaddr_in));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(c->port);
    if (!inet_aton(c->ip, &(serverAddress.sin_addr))) {
        fprintf(stderr, "Error converting address string to bytes.\n");
        return NULL;
    }
    if (connect(c->clientSocketFd, (struct sockaddr*)(&serverAddress), addrlen) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return NULL;
    }
    c->connected = 1;
    return newConnection(c->clientSocketFd);
}

int deleteTCPClient(TCPClient *c) {
    if (c == NULL) {
        fprintf(stderr, "NULL TCPClient instance.\n");
        return -1;
    }
    free(c);
    return 0;
}

#endif /* _client_c_ */
