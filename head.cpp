
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>

using namespace std;

int main() {
    uint8_t destination[6];
    uint8_t source[6];
    uint8_t ether_type[2];

    uint8_t* dp = destination;
    uint8_t* sp = source;
    uint8_t* ep = ether_type;

   
    uint8_t testheader[14] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination MAC
        0x02, 0x00, 0x00, 0x00, 0x00, 0x01, // Source MAC
        0x08, 0x06                          // EtherType: ARP
    };

    
    for (int i = 0; i < 14; i++) {
        if (i < 6) {
            dp[i] = testheader[i];
        }
        else if (i < 12) {
            sp[i - 6] = testheader[i];
        }
        else {
            ep[i - 12] = testheader[i];
        }
    }

    
    cout << hex << uppercase << setfill('0');

    cout << "Destination MAC: ";
    for (int i = 0; i < 6; i++) {
        if (i > 0) cout << ":";
        cout << setw(2) << static_cast<int>(destination[i]);
    }
    cout << '\n';

    cout << "Source MAC: ";
    for (int i = 0; i < 6; i++) {
        if (i > 0) cout << ":";
        cout << setw(2) << static_cast<int>(source[i]);
    }
    cout << '\n';

   
    uint16_t type =
        (static_cast<uint16_t>(ether_type[0]) << 8) |
        static_cast<uint16_t>(ether_type[1]);

    cout << "EtherType: 0x" << setw(4) << type << '\n';

    if (type == 0x0806) {
        cout << "Protocol: ARP\n";
    }
    else if (type == 0x0800) {
        cout << "Protocol: IPv4\n";
    }
    else if (type == 0x86DD) {
        cout << "Protocol: IPv6\n";
    }
    else {
        cout << "Protocol: Unknown\n";
    }

    return 0;
}
