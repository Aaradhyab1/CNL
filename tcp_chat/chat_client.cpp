#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

#define PORT 8081

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server{};
    char buffer[1024];

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server.sin_port = htons(PORT);

    if (connect(clientSocket, (sockaddr*)&server, sizeof(server)) < 0) {
        cout << "Connection failed!" << endl;
        close(clientSocket);
        return 1;
    }

    cout << "Connected to server!" << endl;

    while (true) {
        cout << "You: ";
        cin.getline(buffer, sizeof(buffer));

        send(clientSocket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit") == 0) break;

        int bytes = recv(clientSocket, buffer, 1023, 0);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';
        cout << "Server: " << buffer << endl;

        if (strcmp(buffer, "exit") == 0) break;
    }

    close(clientSocket);
    return 0;
}
