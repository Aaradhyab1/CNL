#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

#define PORT 8080

int main() {
    int serverSocket;
    char buffer[1024];
    sockaddr_in server{}, client{};
    socklen_t clientSize = sizeof(client);

    // Create UDP socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (serverSocket < 0) {
        cout << "Socket creation failed!" << endl;
        return 1;
    }

    // Set server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind socket
    if (::bind(serverSocket, (sockaddr*)&server,
               sizeof(server)) < 0) {
        cout << "Bind failed!" << endl;
        close(serverSocket);
        return 1;
    }

    cout << "UDP Server waiting..." << endl;

    while (true) {
        clientSize = sizeof(client);

        // Receive operation and string
        int bytes = recvfrom(serverSocket, buffer, 1023, 0,
                             (sockaddr*)&client, &clientSize);

        if (bytes < 0) {
            cout << "Receive failed!" << endl;
            break;
        }

        if (bytes == 0)
            continue;

        buffer[bytes] = '\0';

        char operation = buffer[0];
        string result = buffer + 1;

        if (operation == '1') {
            for (int i = 0; i < result.length(); i++) {
                if (result[i] >= 'A' && result[i] <= 'Z')
                    result[i] = result[i] + 32;
            }
        }
        else if (operation == '2') {
            int left = 0;
            int right = (int)result.length() - 1;

            while (left < right) {
                char temp = result[left];
                result[left] = result[right];
                result[right] = temp;
                left++;
                right--;
            }
        }

        // Send result to client
        if (sendto(serverSocket, result.c_str(),
                   result.length(), 0,
                   (sockaddr*)&client, clientSize) < 0) {
            cout << "Send failed!" << endl;
        }

        cout << "Result: " << result << endl;
    }

    close(serverSocket);
    return 0;
}