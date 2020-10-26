#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <endian.h>
#include <tcpdata.h>
#include <connection_common.h>

#ifndef _connection_h_
#define _connection_h_

typedef struct Connection {
    int connectionFd;
} Connection;

Connection *newConnection(int fd);
int closeConnection(Connection *s);

int sendString(Connection *con, char *str, unsigned int size);
int recvString(Connection *con, char *str, unsigned int size);
int sendBinaryData(Connection *con, void *data, unsigned int size);
int recvBinaryData(Connection *con, void *data, unsigned int size);
int sendChar(Connection *con, char *c);
int recvChar(Connection *con, char *c);
int sendUnsignedChar(Connection *con, unsigned char *c);
int recvUnsignedChar(Connection *con, unsigned char *c);
int sendShortInt(Connection *con, short int *si);
int recvShortInt(Connection *con, short int *si);
int sendUnsignedShortInt(Connection *con, unsigned short int *usi);
int recvUnsignedShortInt(Connection *con, unsigned short int *usi);
int sendInt(Connection *con, int *i);
int recvInt(Connection *con, int *i);
int sendUnsignedInt(Connection *con, int *ui);
int recvUnsignedInt(Connection *con, int *ui);
int sendFloat(Connection *con, float *f);
int recvFloat(Connection *con, float *f);

int sendADCVType(Connection *con, AdcvTcpData *data);
int recvADCVType(Connection *con, AdcvTcpData *data);

#endif /* _connection_h_ */
