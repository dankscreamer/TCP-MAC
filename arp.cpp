
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include <cstddef>

using namespace std;

bool arp_parse(const uint8_t* buffer, size_t length) {

    // Step 1: Check that the ARP payload is complete.
    if (buffer == nullptr) {
        cerr << "Error: Null buffer\n";
        return false;
    }

    if (length < 28) {
        cerr << "Error: ARP payload is too short\n";
        return false;
    }

    // Step 2: Parse fixed fields.
    uint16_t hardware_type =
        (static_cast<uint16_t>(buffer[0]) << 8) | buffer[1];

    uint16_t protocol_type =
        (static_cast<uint16_t>(buffer[2]) << 8) | buffer[3];

    uint8_t hardware_address_length = buffer[4];
    uint8_t protocol_address_length = buffer[5];

    uint16_t operation_type =
        (static_cast<uint16_t>(buffer[6]) << 8) | buffer[7];

    // Step 3: Validate fixed fields.
    if (hardware_type != 1) {
        cerr << "Error: Unsupported hardware type\n";
        return false;
    }

    if (protocol_type != 0x0800) {
        cerr << "Error: Unsupported protocol type\n";
        return false;
    }

    if (hardware_address_length != 6 ||
        protocol_address_length != 4) {
        cerr << "Error: Invalid address lengths\n";
        return false;
    }

    if (operation_type != 1 && operation_type != 2) {
        cerr << "Error: Unsupported ARP operation\n";
        return false;
    }

    // Step 4: Extract addresses.
    uint8_t sender_h_address[6];
    uint8_t sender_p_address[4];
    uint8_t target_h_address[6];
    uint8_t target_p_address[4];

    copy(buffer + 8,  buffer + 14, sender_h_address);
    copy(buffer + 14, buffer + 18, sender_p_address);
    copy(buffer + 18, buffer + 24, target_h_address);
    copy(buffer + 24, buffer + 28, target_p_address);

    // Step 5: Display helpers.
    auto print_mac = [](const uint8_t address[]) {
        for (int i = 0; i < 6; i++) {
            if (i != 0) {
                cout << ':';
            }

            cout << hex << setw(2) << setfill('0')
                 << static_cast<unsigned int>(address[i]);
        }

        cout << dec << setfill(' ');
    };

    auto print_ip = [](const uint8_t address[]) {
        for (int i = 0; i < 4; i++) {
            if (i != 0) {
                cout << '.';
            }

            cout << static_cast<unsigned int>(address[i]);
        }
    };

    // Step 6: Print parsed fields.
    cout << "Hardware type: " << hardware_type << '\n';

    cout << "Protocol type: 0x"
         << hex << setw(4) << setfill('0') << protocol_type
         << dec << setfill(' ') << '\n';

    cout << "Hardware address length: "
         << static_cast<unsigned int>(hardware_address_length) << '\n';

    cout << "Protocol address length: "
         << static_cast<unsigned int>(protocol_address_length) << '\n';

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

    return true;
}


int main() {

    // Test 1: Valid synthetic ARP request.
    const uint8_t arp_payload[28] = {
        0x00, 0x01,             // Hardware type: Ethernet
        0x08, 0x00,             // Protocol type: IPv4
        0x06,                   // Hardware address length
        0x04,                   // Protocol address length
        0x00, 0x01,             // Operation: ARP request
        0x02, 0x00, 0x00, 0x00, 0x00, 0x01, // Sender MAC
        0xC0, 0xA8, 0x01, 0x0A, // Sender IP: 192.168.1.10
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Target MAC
        0xC0, 0xA8, 0x01, 0x14  // Target IP: 192.168.1.20
    };

    cout << "=== Valid ARP request ===\n";

    bool success = arp_parse(arp_payload, sizeof(arp_payload));

    cout << (success ? "Test passed\n" : "Test failed\n");

    // Test 2: Truncated payload.
    cout << "\n=== Truncated ARP payload ===\n";

    success = arp_parse(arp_payload, 20);

    cout << (!success ? "Test passed: rejected short payload\n"
                      : "Test failed: accepted short payload\n");

    // Test 3: Unsupported hardware type.
    cout << "\n=== Unsupported hardware type ===\n";

    uint8_t invalid_payload[28];

    copy(arp_payload, arp_payload + 28, invalid_payload);
    invalid_payload[1] = 0x02;

    success = arp_parse(invalid_payload, sizeof(invalid_payload));

    cout << (!success ? "Test passed: rejected invalid hardware type\n"
                      : "Test failed: accepted invalid hardware type\n");

    // Test 4: Unsupported operation.
    cout << "\n=== Unsupported ARP operation ===\n";

    copy(arp_payload, arp_payload + 28, invalid_payload);
    invalid_payload[6] = 0x00;
    invalid_payload[7] = 0x03;

    success = arp_parse(invalid_payload, sizeof(invalid_payload));

    cout << (!success ? "Test passed: rejected invalid operation\n"
                      : "Test failed: accepted invalid operation\n");

    return 0;
}
