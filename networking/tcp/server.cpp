#include <server.hpp>

#ifndef _server_cpp_
#define _server_cpp_

TCPServer::TCPServer(unsigned short int listenPort) {
    this->serverListenPort = listenPort;
    this->serverListenAddress = string("0.0.0.0");
    this->listenSocketFdOpened = false;
    this->createSocket();
    this->bindSocket();
    this->markSocketAsListening();
}

TCPServer::TCPServer(string listenAddress, unsigned short int listenPort) {
    this->serverListenPort = listenPort;
    this->serverListenAddress = listenAddress;
    this->listenSocketFdOpened = false;
    this->createSocket();
    this->bindSocket();
    this->markSocketAsListening();
}

TCPServer::~TCPServer() {
    close(this->listenSocketFd);
    // if ((!this->listenSocketFdOpened) && (close(this->listenSocketFd) == -1))
    //     throw new runtime_error(string(strerror(errno)));
}

void TCPServer::createSocket(void) {
    this->listenSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->listenSocketFd == -1)
        throw new runtime_error(string(strerror(errno)));
    this->listenSocketFdOpened = true;
}

void TCPServer::bindSocket(void) {
    struct sockaddr_in bindAt;
    memset(&bindAt, 0, sizeof(struct sockaddr_in));
    bindAt.sin_family = AF_INET;
    bindAt.sin_port = htons(this->serverListenPort);
    if (!inet_aton(this->serverListenAddress.c_str(), &(bindAt.sin_addr)))
        throw new runtime_error(string("Erro na conversão de bytes para endereço de rede"));
    if (bind(this->listenSocketFd, (struct sockaddr *)&bindAt, sizeof(struct sockaddr_in)) == -1)
        throw new runtime_error(string(strerror(errno)));
}

void TCPServer::markSocketAsListening(void) {
    if (listen(this->listenSocketFd, SOMAXCONN) == -1)
        throw new runtime_error(string(strerror(errno)));
}

Connection *TCPServer::accept_wait(void) {
    struct sockaddr peerAddr;
    socklen_t peerAddrSize;
    memset(&peerAddr, 0, sizeof(struct sockaddr));
    int connectionFd = accept(this->listenSocketFd, &peerAddr, &peerAddrSize);
    if (connectionFd == -1)
        throw new runtime_error(string(strerror(errno)));
    return new Connection(connectionFd);
}

#endif /* _server_cpp_ */
