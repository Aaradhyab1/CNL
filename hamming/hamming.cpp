#include <iostream>
#include <string>
using namespace std;

bool valid(const string& s, size_t n) {
    if (s.size() != n) return false;
    for (char c : s) if (c != '0' && c != '1') return false;
    return true;
}

int main() {
    string data;
    cout << "Enter 4-bit data: ";
    cin >> data;
    if (!valid(data, 4)) { cout << "Invalid input\n"; return 1; }

    int code[8] = {0};
    code[7] = data[0] - '0';
    code[6] = data[1] - '0';
    code[5] = data[2] - '0';
    code[3] = data[3] - '0';
    code[1] = code[3] ^ code[5] ^ code[7];
    code[2] = code[3] ^ code[6] ^ code[7];
    code[4] = code[5] ^ code[6] ^ code[7];

    cout << "Transmitted codeword: ";
    for (int i = 7; i >= 1; i--) cout << code[i];
    cout << "\n";

    // Introduce single-bit error
    int pos;
    cout << "Enter bit position to corrupt (1-7, 0 = none): ";
    cin >> pos;
    if (pos < 0 || pos > 7) { cout << "Invalid position\n"; return 1; }

    int rx[8];
    for (int i = 1; i <= 7; i++) rx[i] = code[i];
    if (pos != 0) rx[pos] ^= 1;

    cout << "Received codeword   : ";
    for (int i = 7; i >= 1; i--) cout << rx[i];
    cout << "\n";

    // Receiver: detect
    int s1 = rx[1] ^ rx[3] ^ rx[5] ^ rx[7];
    int s2 = rx[2] ^ rx[3] ^ rx[6] ^ rx[7];
    int s4 = rx[4] ^ rx[5] ^ rx[6] ^ rx[7];
    int errorPos = s4 * 4 + s2 * 2 + s1;

    cout << "Syndrome (S4S2S1)   : " << s4 << s2 << s1 << "\n";

    // Correct
    if (errorPos != 0) {
        cout << "Error detected at position " << errorPos << "\n";
        rx[errorPos] ^= 1;
    } else {
        cout << "No error detected\n";
    }

    cout << "Corrected codeword  : ";
    for (int i = 7; i >= 1; i--) cout << rx[i];
    cout << "\nExtracted data      : " << rx[7] << rx[6] << rx[5] << rx[3] << "\n";
    cout << (string(1, '0' + rx[7]) + char('0' + rx[6]) + char('0' + rx[5]) + char('0' + rx[3]) == data
             ? "Data recovered correctly\n" : "Recovery FAILED\n");
    return 0;
}