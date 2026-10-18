#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>

using namespace std;

int main() {
    uint8_t hard[2];
    uint8_t ptype[2];
    uint8_t hard_address[1];
    uint8_t protocol_address[1];
    uint8_t sender_h_address[6];
    uint8_t sender_p_address[4];
    uint8_t target_h_address[6];
    uint8_t target_p_address[4];
    uint8_t operation[2];

    uint8_t arp_payload[28] = {
        0x00, 0x01,             // Hardware type: Ethernet
        0x08, 0x00,             // Protocol type: IPv4
        0x06,                   // Hardware address length: 6
        0x04,                   // Protocol address length: 4
        0x00, 0x01,             // Operation: ARP request
        0x02, 0x00, 0x00, 0x00, 0x00, 0x01, // Sender MAC
        0xC0, 0xA8, 0x01, 0x0A, // Sender IP: 192.168.1.10
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Target MAC
        0xC0, 0xA8, 0x01, 0x14  // Target IP: 192.168.1.20
    };

    // Step 1: Parse the fixed fields.
    hard[0] = arp_payload[0];
    hard[1] = arp_payload[1];

    ptype[0] = arp_payload[2];
    ptype[1] = arp_payload[3];

    hard_address[0] = arp_payload[4];
    protocol_address[0] = arp_payload[5];

    operation[0] = arp_payload[6];
    operation[1] = arp_payload[7];

    // Combine two-byte fields using network byte order.
    uint16_t hardware_type =
        (static_cast<uint16_t>(hard[0]) << 8) | hard[1];

    uint16_t protocol_type =
        (static_cast<uint16_t>(ptype[0]) << 8) | ptype[1];

    uint16_t operation_type =
        (static_cast<uint16_t>(operation[0]) << 8) | operation[1];

    // Step 2: Validate the fixed fields.
    if (hardware_type != 1) {
        cerr << "Unsupported hardware type\n";
        return 1;
    }

    if (protocol_type != 0x0800) {
        cerr << "Unsupported protocol type\n";
        return 1;
    }

    if (hard_address[0] != 6 || protocol_address[0] != 4) {
        cerr << "Invalid hardware or protocol address length\n";
        return 1;
    }

    if (operation_type != 1 && operation_type != 2) {
        cerr << "Unsupported ARP operation\n";
        return 1;
    }

    // Step 3: Extract the address fields.
    copy(arp_payload + 8,  arp_payload + 14, sender_h_address);
    copy(arp_payload + 14, arp_payload + 18, sender_p_address);
    copy(arp_payload + 18, arp_payload + 24, target_h_address);
    copy(arp_payload + 24, arp_payload + 28, target_p_address);

    // Step 4: Print MAC addresses in hexadecimal.
    auto print_mac = [](const uint8_t address[]) {
        for (int i = 0; i < 6; i++) {
            if (i != 0) cout << ':';
            cout << hex << setw(2) << setfill('0')
                 << static_cast<unsigned int>(address[i]);
        }
        cout << dec << setfill(' ');
    };

    // Step 5: Print IPv4 addresses in decimal.
    auto print_ip = [](const uint8_t address[]) {
        for (int i = 0; i < 4; i++) {
            if (i != 0) cout << '.';
            cout << static_cast<unsigned int>(address[i]);
        }
    };

    cout << "Hardware type: " << hardware_type << '\n';
    cout << "Protocol type: 0x"
         << hex << setw(4) << setfill('0') << protocol_type
         << dec << setfill(' ') << '\n';

    cout << "Operation: "
         << (operation_type == 1 ? "ARP request" : "ARP reply")
         << '\n';

    cout << "Sender MAC: ";
    print_mac(sender_h_address);
    cout << '\n';

    cout << "Sender IP: ";
    print_ip(sender_p_address);
    cout << '\n';

    cout << "Target MAC: ";
    print_mac(target_h_address);
    cout << '\n';

    cout << "Target IP: ";
    print_ip(target_p_address);
    cout << '\n';

    return 0;
}