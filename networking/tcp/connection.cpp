#include <connection.hpp>

#ifndef _connection_cpp_
#define _connection_cpp_

Connection::Connection(int newConnectionFd) {
    this->connectionFd = newConnectionFd;
}

Connection::~Connection() {
    close(this->connectionFd);
}

Connection *Connection::operator<<(string &str) {
    struct data_t sendData;
    const char *tmpStr = str.c_str();
    sendData.length = strlen(tmpStr) + 1;
    sendData.type = net_string_t;
    for (unsigned int dataIndex = 0; dataIndex < sendData.length; dataIndex += DEFAULT_CONNECTION_BUFFER_SIZE) {
        for (unsigned int bufferIndex = 0; // same type as struct data_t.length
            (bufferIndex < DEFAULT_CONNECTION_BUFFER_SIZE) &&
            ((dataIndex + bufferIndex) < sendData.length); bufferIndex++)
                sendData.data[bufferIndex] = tmpStr[dataIndex + bufferIndex];
        if (send(this->connectionFd, &sendData, sizeof(struct data_t), 0) == -1)
            throw new runtime_error(string(strerror(errno)));
    }
    return this;
}

Connection *Connection::operator>>(string &str) {
    struct data_t recvData;
    char *tmpStr = NULL;

    for (unsigned int dataIndex = 0; ; dataIndex += DEFAULT_CONNECTION_BUFFER_SIZE) {
        if (recv(this->connectionFd, &recvData, sizeof(struct data_t), 0) == -1)
            throw new runtime_error(string(strerror(errno)));
        if (recvData.type != net_string_t)
            throw new runtime_error(string("Incorrect type on I/O operation"));

        if (dataIndex == 0)
            tmpStr = new char[recvData.length];

        for (unsigned int bufferIndex = 0; // same type as struct data_t.length
            (bufferIndex < DEFAULT_CONNECTION_BUFFER_SIZE) &&
            ((dataIndex + bufferIndex) < recvData.length); bufferIndex++)
                tmpStr[dataIndex + bufferIndex] = recvData.data[bufferIndex];

        if (dataIndex >= recvData.length)
            break;
    }

    str = string(tmpStr);

    if (tmpStr != NULL)
        delete tmpStr;
    return this;
}

void Connection::sendBinaryData(void *data, unsigned int size) {
    struct data_t sendData;
    for (unsigned int dataIndex = 0; dataIndex < size; dataIndex += DEFAULT_CONNECTION_BUFFER_SIZE) {
        memset(&sendData, 0, sizeof(struct data_t));
        sendData.type = net_binary_t;
        sendData.length = 0;
        for (unsigned int bufferIndex = 0; // same type as struct data_t.length
            (bufferIndex < DEFAULT_CONNECTION_BUFFER_SIZE) &&
            ((dataIndex + bufferIndex) < size); bufferIndex++, sendData.length++)
                sendData.data[bufferIndex] = ((char*)data)[dataIndex + bufferIndex];
        if (send(this->connectionFd, &sendData, sizeof(struct data_t), 0) == -1)
            throw new runtime_error(string(strerror(errno)));
    }
}

void Connection::recvBinaryData(void *data, unsigned int size) {
    struct data_t recvData;
    for (unsigned int dataIndex = 0; dataIndex < size; dataIndex += DEFAULT_CONNECTION_BUFFER_SIZE) {
        if (recv(this->connectionFd, &recvData, sizeof(struct data_t), 0) == -1)
            throw new runtime_error(string(strerror(errno)));
        if (recvData.type != net_binary_t)
            throw new runtime_error(string("Incorrect type on I/O operation"));

        for (unsigned int bufferIndex = 0; // same type as struct data_t.length
            (bufferIndex < DEFAULT_CONNECTION_BUFFER_SIZE) &&
            ((dataIndex + bufferIndex) < size); bufferIndex++, recvData.length++)
                ((char*)data)[dataIndex + bufferIndex] = recvData.data[bufferIndex];
    }
}

void Connection::sendAdcvTcpData(AdcvTcpData *data) {
    if (data == NULL)
        throw new runtime_error(string("NULL instance of AdcvTcpData"));
    if (send(this->connectionFd, data, sizeof(AdcvTcpData), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
}
    
void Connection::recvAdcvTcpData(AdcvTcpData *data) {
    if (data == NULL)
        throw new runtime_error(string("NULL instance of AdcvTcpData"));
    if (recv(this->connectionFd, data, sizeof(AdcvTcpData), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
}

Connection *Connection::operator<<(bool &b) {
    struct data_t sendData;
    sendData.type = net_bool_t;
    sendData.length = 0;
    ((bool*)(&sendData.data))[0] = b;
    if (send(this->connectionFd, &sendData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    return this;
}

Connection *Connection::operator>>(bool &b) {
    struct data_t recvData;
    if (recv(this->connectionFd, &recvData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    if (recvData.type != net_bool_t)
        throw new runtime_error(string("Incorrect type on I/O operation"));
    b = ((bool*)(&recvData.data))[0];
    return this;
}

Connection *Connection::operator<<(char &c) {
    struct data_t sendData;
    sendData.type = net_char_t;
    sendData.length = 0;
    ((char*)(&sendData.data))[0] = c;
    if (send(this->connectionFd, &sendData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    return this;
}

Connection *Connection::operator>>(char &c) {
    struct data_t recvData;
    if (recv(this->connectionFd, &recvData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    if (recvData.type != net_char_t)
        throw new runtime_error(string("Incorrect type on I/O operation"));
    c = ((char*)(&recvData.data))[0];
    return this;
}

Connection *Connection::operator<<(unsigned char &u_c) {
    struct data_t sendData;
    sendData.type = net_u_char_t;
    sendData.length = 0;
    ((unsigned char*)(&sendData.data))[0] = u_c;
    if (send(this->connectionFd, &sendData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    return this;
}

Connection *Connection::operator>>(unsigned char &u_c) {
    struct data_t recvData;
    if (recv(this->connectionFd, &recvData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    if (recvData.type != net_u_char_t)
        throw new runtime_error(string("Incorrect type on I/O operation"));
    u_c = ((unsigned char*)(&recvData.data))[0];
    return this;
}

Connection *Connection::operator<<(short int &s_i) {
    struct data_t sendData;
    sendData.type = net_short_int_t;
    sendData.length = 0;
    ((short int*)(&sendData.data))[0] = htobe16(s_i);
    if (send(this->connectionFd, &sendData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    return this;
}

Connection *Connection::operator>>(short int &s_i) {
    struct data_t recvData;
    if (recv(this->connectionFd, &recvData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    if (recvData.type != net_short_int_t)
        throw new runtime_error(string("Incorrect type on I/O operation"));
    s_i = be16toh(((short int*)(&recvData.data))[0]);
    return this;
}

Connection *Connection::operator<<(unsigned short int &u_s_i) {
    struct data_t sendData;
    sendData.type = net_u_short_int_t;
    sendData.length = 0;
    ((unsigned short int*)(&sendData.data))[0] = htobe16(u_s_i);
    if (send(this->connectionFd, &sendData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    return this;
}

Connection *Connection::operator>>(unsigned short int &u_s_i) {
    struct data_t recvData;
    if (recv(this->connectionFd, &recvData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    if (recvData.type != net_u_short_int_t)
        throw new runtime_error(string("Incorrect type on I/O operation"));
    u_s_i = be16toh(((unsigned short int*)(&recvData.data))[0]);
    return this;
}

Connection *Connection::operator<<(int &i) {
    struct data_t sendData;
    sendData.type = net_int_t;
    sendData.length = 0;
    ((int*)(&sendData.data))[0] = (int)htobe32(i);
    if (send(this->connectionFd, &sendData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    return this;
}

Connection *Connection::operator>>(int &i) {
    struct data_t recvData;
    if (recv(this->connectionFd, &recvData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    if (recvData.type != net_int_t)
        throw new runtime_error(string("Incorrect type on I/O operation"));
    i = be32toh(((int*)(&recvData.data))[0]);
    return this;
}

Connection *Connection::operator<<(unsigned int &u_i) {
    struct data_t sendData;
    sendData.type = net_u_int_t;
    sendData.length = 0;
    ((unsigned int*)(&sendData.data))[0] = (int)htobe32(u_i);
    if (send(this->connectionFd, &sendData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    return this;
}

Connection *Connection::operator>>(unsigned int &u_i) {
    struct data_t recvData;
    if (recv(this->connectionFd, &recvData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    if (recvData.type != net_u_int_t)
        throw new runtime_error(string("Incorrect type on I/O operation"));
    u_i = be32toh(((unsigned int*)(&recvData.data))[0]);
    return this;
}

Connection *Connection::operator<<(float &f) {
    struct data_t sendData;
    sendData.type = net_float_t;
    sendData.length = 0;
    ((float*)(&sendData.data))[0] = (float)htobe32(f);
    if (send(this->connectionFd, &sendData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    return this;
}

Connection *Connection::operator>>(float &f) {
    struct data_t recvData;
    if (recv(this->connectionFd, &recvData, sizeof(struct data_t), 0) == -1)
        throw new runtime_error(string(strerror(errno)));
    if (recvData.type != net_float_t)
        throw new runtime_error(string("Incorrect type on I/O operation"));
    f = be32toh(((float*)(&recvData.data))[0]);
    return this;
}

#endif /* _connection_cpp_ */
