#include <server.h>

#ifndef _server_c_
#define _server_c_

static void openSocket(TCPServer *s) {
    s->listenSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (s->listenSocketFd == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return;
    }
    s->isSocketFdOpened = 1;
}

static void bindSocket(TCPServer *s) {
    struct sockaddr_in bindAt;
    memset(&bindAt, 0, sizeof(struct sockaddr_in));
    bindAt.sin_family = AF_INET;
    bindAt.sin_port = htons(s->serverListenPort);
    if (!inet_aton(s->serverListenAddress, &(bindAt.sin_addr))) {
        fprintf(stderr, "%s\n", strerror(errno));
        return;
    }
    if (bind(s->listenSocketFd, (struct sockaddr *)&bindAt, sizeof(struct sockaddr_in) == -1)) {
        fprintf(stderr, "%s\n", strerror(errno));
        return;
    }
    s->isSocketBinded = 1;
}

static void listenOnSocket(TCPServer *s) {
    if (listen(s->listenSocketFd, SOMAXCONN) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return;
    }
    s->isSocketListening = 1;
}

TCPServer *newTCPServer(unsigned short int port) {
    TCPServer *s = NULL;
    s = (TCPServer*)malloc(sizeof(TCPServer));
    if (s == NULL)
        fprintf(stderr, "%s\n", strerror(errno));
        return NULL;
    memset(s, 0, sizeof(TCPServer));
    s->serverListenAddress = "0.0.0.0";
    s->serverListenPort = port;
    openSocket(s);
    if (!(s->isSocketFdOpened)) {
        free(s);
        return NULL;
    }
    bindSocket(s);
    if (!(s->isSocketBinded)) {
        free(s);
        return NULL;
    }
    listenOnSocket(s);
    if (!(s->isSocketListening)) {
        free(s);
        return NULL;
    }
    return s;
}

Connection *accept_wait(TCPServer *s) {
    struct sockaddr peerAddr;
    socklen_t peerAddrSize;
    memset(&peerAddr, 0, sizeof(struct sockaddr));
    int connectionFd = accept(s->listenSocketFd, &peerAddr, &peerAddrSize);
    if (connectionFd == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return NULL;
    }
    return newConnection(connectionFd);
}

int deleteTCPServer(TCPServer *s) {
    if (s == NULL) {
        fprintf(stderr, "%s\n", "NULL TCPServer instance.");
        return -1;
    }
    if (close(s->listenSocketFd) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -2;
    }
    free(s);
    return 0;
}

#endif /* _server_c_ */
