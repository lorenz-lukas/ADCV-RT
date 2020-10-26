#include <client.hpp>

#ifndef _client_cpp_
#define _client_cpp_

TCPClient::TCPClient(string serverIp, unsigned short int serverPort) {
    this->ip = serverIp;
    this->port = serverPort;
    this->connected = false;
    this->clientSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->clientSocketFd == -1)
        throw new runtime_error(string(strerror(errno)));
}

Connection *TCPClient::connect_client(void) {
    if (this->connected)
        throw new runtime_error(string("TCPClient instance already connected."));

    struct sockaddr_in serverAddress;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    memset(&serverAddress, 0, sizeof(struct sockaddr_in));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->port);

    if (!inet_aton(this->ip.c_str(), &(serverAddress.sin_addr)))
        throw new runtime_error(string("Error converting address string to bytes."));

    if (connect(this->clientSocketFd, (struct sockaddr*)&serverAddress, addrlen) == -1)
        throw new runtime_error(string(strerror(errno)));

    this->connected = true;

    return new Connection(this->clientSocketFd);
}

#endif /* _client_cpp_ */
