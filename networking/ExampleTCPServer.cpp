#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <server.hpp>

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char **argv, char **env) {
    srand(time(NULL));
    TCPServer *srv = NULL;
    Connection *con = NULL;
    try {
        srv = new TCPServer(10000);
        con = srv->accept_wait();
    }
    catch(runtime_error *e) {
        cerr << e->what() << endl;
    }

    if (con != NULL) {
        int tmp = 2123456;
        cout << "Enviando: " << tmp << endl;
        *con << tmp;
        cout << "Enviado" << endl;
    }


    int numbersToSend = 128;
    int *randomNumbers = NULL;
    randomNumbers = new int[numbersToSend];

    for (int i = 0; i < numbersToSend; i++)
        randomNumbers[i] = rand();
    
    if (con != NULL) {
        cout << "Enviando os seguintes números aleatórios:" << endl;
        for (int i = 0; i < numbersToSend; i++) {
            cout << randomNumbers[i] << ", ";
            if (i % 4 == 0)
                cout << endl;
        }

        con->sendBinaryData((void*)randomNumbers, sizeof(int) * numbersToSend);
    }

    if (randomNumbers != NULL) {
        delete randomNumbers;
        randomNumbers = NULL;
    }

    if (con != NULL) {
        string tmp("Aenean blandit tellus sed dignissim tempor. Aenean posuere tincidunt dui dignissim rhoncus. Duis vel libero at est maximus cursus in eu dolor. Nulla quis massa velit. Duis metus est, feugiat id turpis vulputate, mattis egestas sapien. Aenean commodo elit vitae ex ullamcorper finibus. Duis risus libero, scelerisque et purus ut, gravida convallis magna. Nunc eget aliquam ligula. Mauris vel libero ut nunc pharetra fermentum. Curabitur congue ullamcorper lacus, eget molestie ex tincidunt in. Nullam vitae arcu congue felis malesuada dictum.");
        cout << "Try to send a string." << endl;
        *con << tmp;
        cout << "Sended string: " << tmp << endl;
    }

    if (srv != NULL) { // Closing server socket
        delete srv;
        srv = NULL;
    }

    if (con != NULL) { // Closing connection
        delete con;
        con = NULL;
    }

    return 0;
}
