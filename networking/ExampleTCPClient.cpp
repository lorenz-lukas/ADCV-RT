#include <iostream>
#include <client.hpp>

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char **argv, char **env) {
    TCPClient *cli = NULL;
    Connection *con = NULL;
    try {
        cli = new TCPClient("127.0.0.1", 10000);
        con = cli->connect_client();
    }
    catch(runtime_error *e) {
        cerr << e->what() << endl;
    }

    if (con != NULL) {
        int tmp;
        *con >> tmp;
        cout << "Recebido: " << tmp << endl;
    }

    int numbersToRecv = 128;
    int *randomRcevdNumbers = NULL;
    randomRcevdNumbers = new int[numbersToRecv];

    if (con != NULL) {
        con->recvBinaryData((void*)randomRcevdNumbers, sizeof(int) * numbersToRecv);
        
        cout << "Os seguintes números foram recebidos: " << endl;
        for (int i = 0; i < numbersToRecv; i++) {
            cout << randomRcevdNumbers[i] << ", ";
            if (i % 4 == 0)
                cout << endl;
        }
    }

    if (randomRcevdNumbers != NULL) {
        delete randomRcevdNumbers;
        randomRcevdNumbers = NULL;
    }

    if (con != NULL) {
        string tmp;
        cout << "Try to recv a string." << endl;
        *con >> tmp;
        cout << "Recved string: " << tmp << endl;
        cout << "Recved string length: " << tmp.length() << endl;
    }

    if (cli != NULL) {
        delete cli;
        cli = NULL;
    }

    if (con != NULL) { // Closing connection
        delete con;
        con = NULL;
    }

    return 0;
}
