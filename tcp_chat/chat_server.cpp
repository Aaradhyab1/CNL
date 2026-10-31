#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

#define PORT 8081

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server{}, client{};
    socklen_t size = sizeof(client);
    char buffer[1024];

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (::bind(serverSocket, (sockaddr*)&server, sizeof(server)) < 0) {
        cout << "Bind failed!" << endl;
        close(serverSocket);
        return 1;
    }

    listen(serverSocket, 5);
    cout << "Server waiting..." << endl;

    int clientSocket = accept(serverSocket, (sockaddr*)&client, &size);
    cout << "Client connected!" << endl;

    while (true) {
        int bytes = recv(clientSocket, buffer, 1023, 0);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';
        cout << "Client: " << buffer << endl;

        if (strcmp(buffer, "exit") == 0) break;

        cout << "Enter reply: ";
        cin.getline(buffer, sizeof(buffer));

        send(clientSocket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit") == 0) break;
    }

    close(clientSocket);
    close(serverSocket);
    return 0;
}