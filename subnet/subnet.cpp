#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

vector<int> parseIP(string ip) {
    vector<int> parts;
    string temp;
    stringstream ss(ip);
    while (getline(ss, temp, '.'))
        parts.push_back(stoi(temp));
    return parts;
}

void printIP(unsigned int ip) {
    cout << ((ip >> 24) & 255) << "."
         << ((ip >> 16) & 255) << "."
         << ((ip >> 8) & 255) << "."
         << (ip & 255);
}

int main() {
    string ip;
    int requiredSubnets;

    cout << "Enter IP address: ";
    cin >> ip;
    cout << "Enter number of required subnets: ";
    cin >> requiredSubnets;

    vector<int> addr = parseIP(ip);

    // Guards
    if (addr.size() != 4) {
        cout << "Invalid IP address.\n";
        return 0;
    }
    for (int i = 0; i < 4; i++) {
        if (addr[i] < 0 || addr[i] > 255) {
            cout << "Invalid IP address.\n";
            return 0;
        }
    }
    if (requiredSubnets < 1) {
        cout << "Invalid number of subnets.\n";
        return 0;
    }

    // Class detection
    char ipClass;
    int defaultBits;

    if (addr[0] >= 1 && addr[0] <= 127) {
        ipClass = 'A';
        defaultBits = 8;
    }
    else if (addr[0] >= 128 && addr[0] <= 191) {
        ipClass = 'B';
        defaultBits = 16;
    }
    else if (addr[0] >= 192 && addr[0] <= 223) {
        ipClass = 'C';
        defaultBits = 24;
    }
    else {
        cout << "Class D/E address. Not supported.\n";
        return 0;
    }

    cout << "\nIP Class: " << ipClass << endl;

    // Borrowed bits (internal only)
    int borrowedBits = 0;
    int possibleSubnets = 1;
    while (possibleSubnets < requiredSubnets) {
        borrowedBits++;
        possibleSubnets *= 2;
    }

    int hostBits = 32 - (defaultBits + borrowedBits);

    if (hostBits < 2) {
        cout << "Too many subnets. Not enough host bits.\n";
        return 0;
    }

    int addressesPerSubnet = 1 << hostBits;

    // Mask
    unsigned int mask = 0xFFFFFFFF << hostBits;

    // Network address
    unsigned int ipValue = ((unsigned int)addr[0] << 24) |
                           ((unsigned int)addr[1] << 16) |
                           ((unsigned int)addr[2] << 8) |
                            (unsigned int)addr[3];

    unsigned int network = ipValue & mask;

    cout << "\n-----------------------------------------------\n";

    for (int i = 0; i < possibleSubnets; i++) {
        unsigned int subnetNetwork = network + i * addressesPerSubnet;
        unsigned int broadcast = subnetNetwork + addressesPerSubnet - 1;

        cout << "\nSubnet " << i + 1;
        cout << "\nNetwork Address : "; printIP(subnetNetwork);
        cout << "\nSubnet Mask     : "; printIP(mask);
        cout << "\nUsable Hosts    : " << addressesPerSubnet - 2;
        cout << "\nFirst IP        : "; printIP(subnetNetwork + 1);
        cout << "\nLast IP         : "; printIP(broadcast - 1);
        cout << "\nBroadcast       : "; printIP(broadcast);
        cout << "\n-----------------------------------------------\n";
    }
    return 0;
}