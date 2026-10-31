#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket < 0) {
        perror("Socket failed");
        return 1;
    }

    sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8082);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddress,
                sizeof(serverAddress)) < 0) {
        perror("Connection failed");
        close(clientSocket);
        return 1;
    }

    cout << "Connected to calculator server!" << endl;

    double a, b, result;
    char op;

    while (true) {
        cout << "\nEnter first number: ";
        if (!(cin >> a)) break;

        cout << "Enter operator (+ - * /): ";
        if (!(cin >> op)) break;

        cout << "Enter second number: ";
        if (!(cin >> b)) break;

        if (op == '/' && b == 0) {
            cout << "Cannot divide by zero!" << endl;
            continue;
        }

        if (send(clientSocket, &a, sizeof(a), 0) < 0) break;
        if (send(clientSocket, &op, sizeof(op), 0) < 0) break;
        if (send(clientSocket, &b, sizeof(b), 0) < 0) break;

        if (recv(clientSocket, &result, sizeof(result), MSG_WAITALL)
            != sizeof(result)) break;

        cout << "Result: " << result << endl;

        char choice;
        cout << "Continue? (y/n): ";
        cin >> choice;

        if (choice != 'y' && choice != 'Y') break;
    }

    close(clientSocket);
    return 0;
}