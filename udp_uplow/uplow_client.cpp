
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

#define PORT 8080

int main() {
    int clientSocket;
    char buffer[1024];

    sockaddr_in server{};
    socklen_t serverSize = sizeof(server);

    // Create UDP socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (clientSocket < 0) {
        cout << "Socket creation failed!" << endl;
        return 1;
    }

    // Set server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    string message;

    // Take string input
    cout << "Enter a string: ";
    getline(cin, message);

    // Send string to server
    sendto(clientSocket, message.c_str(),
           message.length(), 0,
           (sockaddr*)&server, serverSize);

    // Receive converted string
    int bytes = recvfrom(clientSocket, buffer, 1023, 0,
                         (sockaddr*)&server, &serverSize);

    if (bytes < 0) {
        cout << "Receive failed!" << endl;
    }
    else {
        buffer[bytes] = '\0';
        cout << "Uppercase string: " << buffer << endl;
    }

    // Close socket
    close(clientSocket);

    return 0;
}
