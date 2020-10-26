#include <iostream>
#include <string>
#include <stdexcept>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <connection.hpp>

#ifndef _server_hpp_
#define _server_hpp_

using std::string;
using std::runtime_error;

using std::cout;
using std::endl;

class TCPServer {
    string serverListenAddress;
    unsigned short int serverListenPort;
    int listenSocketFd;
    bool listenSocketFdOpened;

    void createSocket(void);
    void bindSocket(void);
    void markSocketAsListening(void);

public:
    TCPServer(unsigned short int listenPort);
    TCPServer(string listenAddress, unsigned short int listenPort);
    ~TCPServer();

    Connection *accept_wait(void);
};

#endif /* _server_hpp_ */
