#include <iostream>
#include <string>
#include <stdexcept>
#include <endian.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <tcpdata.h>
#include <connection_common.h>

#ifndef _connection_hpp_
#define _connection_hpp_

using std::string;
using std::runtime_error;

class Connection {
    int connectionFd;

public:
    Connection(int newConnectionFd);
    ~Connection();

    /**
     * Send arbitrary sized string.
    */
    Connection *operator<<(string &str);

    /**
     * recv arbitrary sized string.
    */
    Connection *operator>>(string &str);

    /**
     * Send arbitrary sized binary data.
    */
    void sendBinaryData(void *data, unsigned int size);

    /**
     * Recv arbitrary sized binary data. 
    */
    void recvBinaryData(void *data, unsigned int size);

    void sendAdcvTcpData(AdcvTcpData *data);
    
    void recvAdcvTcpData(AdcvTcpData *data);

    /**
     * Send a bool.
    */
    Connection *operator<<(bool &b);

    /**
     * Recv a bool.
    */
    Connection *operator>>(bool &b);

    /**
     * Send a char.
    */
    Connection *operator<<(char &c);

    /**
     * Recv a char.
    */
    Connection *operator>>(char &c);

    /**
     * Send a unsigned char.
    */
    Connection *operator<<(unsigned char &u_c);

    /**
     * Recv a unsigned char.
    */
    Connection *operator>>(unsigned char &u_c);

    /**
     * Send a short int.
    */
    Connection *operator<<(short int &s_i);

    /**
     * Recv a short int.
    */
    Connection *operator>>(short int &s_i);

    /**
     * Send a unsigned short int.
    */
    Connection *operator<<(unsigned short int &u_s_i);

    /**
     * Recv a unsigned short int.
    */
    Connection *operator>>(unsigned short int &u_s_i);

    /**
     * Send a int.
    */
    Connection *operator<<(int &i);

    /**
     * Recv a int.
    */
    Connection *operator>>(int &i);

    /**
     * Send a unsigned int.
    */
    Connection *operator<<(unsigned int &i);

    /**
     * Recv a unsigned int.
    */
    Connection *operator>>(unsigned int &i);

    Connection *operator<<(float &f);

    Connection *operator>>(float &f);
};

#endif /* _connection_hpp_ */
