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

    int choice;
    string message;

    cout << "1. Uppercase to Lowercase" << endl;
    cout << "2. Reverse the String" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore();

    cout << "Enter string: ";
    getline(cin, message);

    // Add operation number before the string
    string data = char('0' + choice) + message;

    sendto(clientSocket, data.c_str(),
           data.length(), 0,
           (sockaddr*)&server, serverSize);

    // Receive result
    int bytes = recvfrom(clientSocket, buffer, 1023, 0,
                         (sockaddr*)&server, &serverSize);

    if (bytes < 0) {
        cout << "Receive failed!" << endl;
    } else {
        buffer[bytes] = '\0';
        cout << "Result: " << buffer << endl;
    }

    close(clientSocket);
    return 0;
}