#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0) {
        perror("Socket failed");
        return 1;
    }

    sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8082);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (::bind(serverSocket, (sockaddr*)&serverAddress,
               sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        close(serverSocket);
        return 1;
    }

    cout << "Calculator server waiting..." << endl;

    int clientSocket = accept(serverSocket, nullptr, nullptr);

    if (clientSocket < 0) {
        perror("Accept failed");
        close(serverSocket);
        return 1;
    }

    cout << "Client connected!" << endl;

    double a, b, result;
    char op;

    while (true) {
        if (recv(clientSocket, &a, sizeof(a), MSG_WAITALL) != sizeof(a)) break;
        if (recv(clientSocket, &op, sizeof(op), MSG_WAITALL) != sizeof(op)) break;
        if (recv(clientSocket, &b, sizeof(b), MSG_WAITALL) != sizeof(b)) break;

        switch (op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/': result = (b != 0) ? a / b : 0; break;
            default: result = 0;
        }

        cout << "Calculation: " << a << " " << op
             << " " << b << " = " << result << endl;

        if (send(clientSocket, &result, sizeof(result), 0) < 0)
            break;
    }

    close(clientSocket);
    close(serverSocket);
    return 0;
}
