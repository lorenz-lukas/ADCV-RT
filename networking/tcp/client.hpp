#include <iostream>
#include <string>
#include <stdexcept>
#include <connection.hpp>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#ifndef _client_hpp_
#define _client_hpp_

using std::string;
using std::runtime_error;

class TCPClient {
    string ip;
    unsigned short int port;
    int clientSocketFd;
    bool connected;

public:
    TCPClient(string serverIp, unsigned short int serverPort);

    Connection *connect_client(void);
};

#endif /* _client_hpp_ */
