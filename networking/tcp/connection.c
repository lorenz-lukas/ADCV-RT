#include <connection.h>

#ifndef _connection_c_
#define _connection_c_

Connection *newConnection(int fd) {
    Connection *c = NULL;
    c = (Connection*)malloc(sizeof(Connection));
    if (c == NULL) {
        fprintf(stderr, "%s\n", strerror(errno));
        return NULL;
    }
    memset(c, 0, sizeof(Connection));
    c->connectionFd = fd;
    return c;
}

int closeConnection(Connection *s) {
    if (s == NULL) {
        fprintf(stderr, "NULL Connection instance.\n");
        return -1;
    }
    if (close(s->connectionFd) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -2;
    }
    return 0;
}

int sendString(Connection *con, char *str, unsigned int size) {
    int sended = 0;
    struct data_t sendData;
    sendData.length = size;
    sendData.type = net_string_t;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (str == NULL) {
        fprintf(stderr, "NULL string reference\n.");
        return -2;
    }

    for (unsigned int dataIndex = 0; dataIndex < sendData.length; dataIndex += DEFAULT_CONNECTION_BUFFER_SIZE) {
        for (unsigned int bufferIndex = 0;
            (bufferIndex < DEFAULT_CONNECTION_BUFFER_SIZE) &&
            (dataIndex + bufferIndex) < sendData.length; bufferIndex++, sended++)
                sendData.data[bufferIndex] = str[dataIndex + bufferIndex];
        if (send(con->connectionFd, &sendData, sizeof(struct data_t), 0)) {
            fprintf(stderr, "%s\n", strerror(errno));
            return -3;
        }
    }

    return sended;
}

int recvString(Connection *con, char *str, unsigned int size) {
    int recived = 0;
    struct data_t recvData;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (str == NULL) {
        fprintf(stderr, "NULL string reference\n.");
        return -2;
    }

    for (unsigned int dataIndex = 0; ; dataIndex += DEFAULT_CONNECTION_BUFFER_SIZE) {
        memset(&recvData, 0, sizeof(struct data_t));
        if (recv(con->connectionFd, &recvData, sizeof(struct data_t), 0) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            return -3;
        }
        if (recvData.type != net_string_t) {
            fprintf(stderr, "Incorrect type on I/O operation.\n");
            return -4;
        }
        for (unsigned int bufferIndex = 0; // same type as struct data_t.length
            (bufferIndex < DEFAULT_CONNECTION_BUFFER_SIZE) &&
            ((dataIndex + bufferIndex) < recvData.length); bufferIndex++, recived++)
                str[dataIndex + bufferIndex] = recvData.data[bufferIndex];
        if (dataIndex >= recvData.length)
            break;
    }

    return recived;
}

int sendBinaryData(Connection *con, void *data, unsigned int size) {
    int sended = 0;
    struct data_t sendData;
    sendData.type = net_binary_t;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (data == NULL) {
        fprintf(stderr, "NULL data reference\n.");
        return -2;
    }
    for (unsigned int dataIndex = 0; dataIndex < size; dataIndex += DEFAULT_CONNECTION_BUFFER_SIZE) {
        for (unsigned int bufferIndex = 0;
            (bufferIndex < DEFAULT_CONNECTION_BUFFER_SIZE) &&
            ((dataIndex + bufferIndex) < size); bufferIndex++, sendData.length++, sended++)
                sendData.data[bufferIndex] = ((char*)data)[dataIndex + bufferIndex];
        if (send(con->connectionFd, &sendData, sizeof(struct data_t), 0) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            return -3;
        }
    }
    return sended;
}

int recvBinaryData(Connection *con, void *data, unsigned int size) {
    int recved = 0;
    struct data_t recvData;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (data == NULL) {
        fprintf(stderr, "NULL data reference\n.");
        return -2;
    }

    for (unsigned int dataIndex = 0; dataIndex < size; dataIndex += DEFAULT_CONNECTION_BUFFER_SIZE) {
        memset(&recvData, 0, sizeof(struct data_t));
        if (recv(con->connectionFd, &recvData, sizeof(struct data_t), 0) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            return -3;
        }
        if (recvData.type != net_binary_t) {
            fprintf(stderr, "Incorrect type on I/O operation.\n");
            fprintf(stderr, "recvData.type = %d\n", (int)recvData.type);
            return -4;
        }
        for (unsigned int bufferIndex = 0;
            (bufferIndex < DEFAULT_CONNECTION_BUFFER_SIZE) &&
            ((dataIndex + bufferIndex) < size); bufferIndex++, recvData.length++, recved++)
            ((char*)data)[dataIndex + bufferIndex] = recvData.data[bufferIndex];
    }
    return recved;
}

int sendChar(Connection *con, char *c) {
    struct data_t sendData;
    sendData.type = net_char_t;
    sendData.length = 0;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (c == NULL) {
        fprintf(stderr, "NULL char reference\n.");
        return -2;
    }
    ((char*)(&sendData.data))[0] = *c;
    if (send(con->connectionFd, &sendData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    return 0;
}

int recvChar(Connection *con, char *c) {
    struct data_t recvData;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (c == NULL) {
        fprintf(stderr, "NULL char reference\n.");
        return -2;
    }
    memset(&recvData, 0, sizeof(struct data_t));
    if (recv(con->connectionFd, &recvData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    if (recvData.type != net_char_t) {
        fprintf(stderr, "Incorrect type on I/O operation.\n");
        return -4;
    }
    *c = ((char*)(&recvData.data))[0];
    return 0;
}

int sendUnsignedChar(Connection *con, unsigned char *c) {
    struct data_t sendData;
    sendData.type = net_u_char_t;
    sendData.length = 0;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (c == NULL) {
        fprintf(stderr, "NULL unsigned char reference\n.");
        return -2;
    }
    ((unsigned char*)(&sendData.data))[0] = *c;
    if (send(con->connectionFd, &sendData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    return 0;
}

int recvUnsignedChar(Connection *con, unsigned char *c) {
    struct data_t recvData;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (c == NULL) {
        fprintf(stderr, "NULL unsigned char reference\n.");
        return -2;
    }
    memset(&recvData, 0, sizeof(struct data_t));
    if (recv(con->connectionFd, &recvData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    if (recvData.type != net_u_char_t) {
        fprintf(stderr, "Incorrect type on I/O operation.\n");
        return -4;
    }
    *c = ((unsigned char*)(&recvData.data))[0];
    return 0;
}

int sendShortInt(Connection *con, short int *si) {
    struct data_t sendData;
    sendData.type = net_short_int_t;
    sendData.length = 0;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (si == NULL) {
        fprintf(stderr, "NULL short int reference\n.");
        return -2;
    }
    ((short int*)(&sendData.data))[0] = (short int)htobe16(*si);
    if (send(con->connectionFd, &sendData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    return 0;
}

int recvShortInt(Connection *con, short int *si) {
    struct data_t recvData;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (si == NULL) {
        fprintf(stderr, "NULL short int reference\n.");
        return -2;
    }
    memset(&recvData, 0, sizeof(struct data_t));
    if (recv(con->connectionFd, &recvData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    if (recvData.type != net_short_int_t) {
        fprintf(stderr, "Incorrect type on I/O operation.\n");
        return -4;
    }
    *si = be16toh(((short int*)(&recvData.data))[0]);
    return 0;
}

int sendUnsignedShortInt(Connection *con, unsigned short int *usi) {
    struct data_t sendData;
    sendData.type = net_u_short_int_t;
    sendData.length = 0;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (usi == NULL) {
        fprintf(stderr, "NULL unsigned short int reference\n.");
        return -2;
    }
    ((unsigned short int*)(&sendData.data))[0] = (short int)htobe16(*usi);
    if (send(con->connectionFd, &sendData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    return 0;
}

int recvUnsignedShortInt(Connection *con, unsigned short int *usi) {
    struct data_t recvData;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (usi == NULL) {
        fprintf(stderr, "NULL short int reference\n.");
        return -2;
    }
    memset(&recvData, 0, sizeof(struct data_t));
    if (recv(con->connectionFd, &recvData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    if (recvData.type != net_u_short_int_t) {
        fprintf(stderr, "Incorrect type on I/O operation.\n");
        return -4;
    }
    *usi = be16toh(((unsigned short int*)(&recvData.data))[0]);
    return 0;
}

int sendInt(Connection *con, int *i) {
    struct data_t sendData;
    sendData.type = net_int_t;
    sendData.length = 0;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (i == NULL) {
        fprintf(stderr, "NULL int reference\n.");
        return -2;
    }
    ((int*)(&sendData.data))[0] = (int)htobe32(*i);
    if (send(con->connectionFd, &sendData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    return 0;
}

int recvInt(Connection *con, int *i) {
    struct data_t recvData;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (i == NULL) {
        fprintf(stderr, "NULL int reference\n.");
        return -2;
    }
    memset(&recvData, 0, sizeof(struct data_t));
    if (recv(con->connectionFd, &recvData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    if (recvData.type != net_int_t) {
        fprintf(stderr, "Incorrect type on I/O operation.\n");
        return -4;
    }
    *i = be32toh(((int*)(&recvData.data))[0]);
    return 0;
}

int sendUnsignedInt(Connection *con, int *ui) {
    struct data_t sendData;
    sendData.type = net_u_int_t;
    sendData.length = 0;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (ui == NULL) {
        fprintf(stderr, "NULL unsigned int reference\n.");
        return -2;
    }
    ((unsigned int*)(&sendData.data))[0] = (unsigned int)htobe32(*ui);
    if (send(con->connectionFd, &sendData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    return 0;
}

int recvUnsignedInt(Connection *con, int *ui) {
    struct data_t recvData;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (ui == NULL) {
        fprintf(stderr, "NULL unsigned int reference\n.");
        return -2;
    }
    memset(&recvData, 0, sizeof(struct data_t));
    if (recv(con->connectionFd, &recvData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    if (recvData.type != net_u_int_t) {
        fprintf(stderr, "Incorrect type on I/O operation.\n");
        return -4;
    }
    *ui = be32toh(((unsigned int*)(&recvData.data))[0]);
    return 0;
}

int sendFloat(Connection *con, float *f) {
    struct data_t sendData;
    sendData.type = net_float_t;
    sendData.length = 0;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (f == NULL) {
        fprintf(stderr, "NULL float reference\n.");
        return -2;
    }
    ((float*)(&sendData.data))[0] = (float)htobe32(*f);
    if (send(con->connectionFd, &sendData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    return 0;
}

int recvFloat(Connection *con, float *f) {
    struct data_t recvData;
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (f == NULL) {
        fprintf(stderr, "NULL float reference\n.");
        return -2;
    }
    memset(&recvData, 0, sizeof(struct data_t));
    if (recv(con->connectionFd, &recvData, sizeof(struct data_t), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    if (recvData.type != net_float_t) {
        fprintf(stderr, "Incorrect type on I/O operation.\n");
        return -4;
    }
    *f = be32toh(((float*)(&recvData.data))[0]);
    return 0;
}

int sendADCVType(Connection *con, AdcvTcpData *data) {
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (data == NULL) {
        fprintf(stderr, "NULL AdcvTcpData reference\n.");
        return -2;
    }
    if (send(con->connectionFd, data, sizeof(AdcvTcpData), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    return 0;
}

int recvADCVType(Connection *con, AdcvTcpData *data) {
    if (con == NULL) {
        fprintf(stderr, "NULL connection instance\n.");
        return -1;
    }
    if (data == NULL) {
        fprintf(stderr, "NULL AdcvTcpData reference\n.");
        return -2;
    }
    if (recv(con->connectionFd, data, sizeof(AdcvTcpData), 0) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -3;
    }
    return 0;
}

#endif /* _connection_c_ */
