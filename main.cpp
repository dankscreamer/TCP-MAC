
#include <iostream>
#include <array>
#include <cstdint>
#include <iomanip>

#include "./arp.h"
#include "./ethernet.h"
#include "./ipv4.h"

using namespace std;

int main() {
    ipv4 original{};

    original.version = 4;
    original.ihl = 5;
    original.dscsp_ecn = 0;
    original.total_length = 60;
    original.identification = 0x1234;
    original.flag = 2;
    original.fragment_offset = 0;
    original.ttl = 64;
    original.protocol = 6;
    original.header_checksum = 0;
    original.source_ip = {192, 168, 1, 10};
    original.destination_ip = {192, 168, 1, 20};

    array<uint8_t, 20> serialized{};

    serialise_ipv4_header(original, serialized);

    cout << hex << uppercase << setfill('0');

    uint16_t stored_checksum =
        (static_cast<uint16_t>(serialized[10]) << 8) |
        serialized[11];

    cout << "Checksum in header: 0x"
         << setw(4) << stored_checksum << endl;

    cout << "Checksum validation: "
         << (checksum(serialized) == stored_checksum ? "PASS" : "FAIL")
         << endl;

    cout << "=== Serialized IPv4 Header (20 bytes) ===\n";

    for (size_t i = 0; i < serialized.size(); i++) {
        if (i % 4 == 0) {
            cout << "\n" << setw(2) << i << ": ";
        }

        cout << setw(2) << static_cast<int>(serialized[i]) << " ";
    }

    cout << "\n\n";

    ipv4 parsed{};
    parse_ipv4_header(serialized, parsed);

    bool match =
        original.version == parsed.version &&
        original.ihl == parsed.ihl &&
        original.dscsp_ecn == parsed.dscsp_ecn &&
        original.total_length == parsed.total_length &&
        original.identification == parsed.identification &&
        original.flag == parsed.flag &&
        original.fragment_offset == parsed.fragment_offset &&
        original.ttl == parsed.ttl &&
        original.protocol == parsed.protocol &&
        stored_checksum == parsed.header_checksum &&
        original.source_ip == parsed.source_ip &&
        original.destination_ip == parsed.destination_ip;

    cout << dec;
    cout << (match
                 ? "PASS: IPv4 round-trip test\n"
                 : "FAIL: IPv4 round-trip test\n");

    cout << "\n=== Parsed IPv4 Header ===\n";
    cout << "Version:           " << static_cast<int>(parsed.version) << '\n';
    cout << "IHL:               " << static_cast<int>(parsed.ihl) << '\n';
    cout << "Header length:     " << static_cast<int>(parsed.ihl) * 4
         << " bytes\n";
    cout << "DSCP/ECN:          " << static_cast<int>(parsed.dscsp_ecn) << '\n';
    cout << "Total length:      " << parsed.total_length << '\n';

    cout << hex << uppercase << setfill('0');
    cout << "Identification:    0x" << setw(4) << parsed.identification << '\n';
    cout << "Header checksum:   0x" << setw(4) << parsed.header_checksum << '\n';

    cout << dec;
    cout << "Flags:             " << static_cast<int>(parsed.flag) << '\n';
    cout << "Fragment offset:   " << parsed.fragment_offset << '\n';
    cout << "TTL:               " << static_cast<int>(parsed.ttl) << '\n';
    cout << "Protocol:          " << static_cast<int>(parsed.protocol) << '\n';

    cout << "Source IP:         ";
    for (size_t i = 0; i < parsed.source_ip.size(); i++) {
        if (i > 0) cout << ".";
        cout << static_cast<int>(parsed.source_ip[i]);
    }
    cout << '\n';

    cout << "Destination IP:    ";
    for (size_t i = 0; i < parsed.destination_ip.size(); i++) {
        if (i > 0) cout << ".";
        cout << static_cast<int>(parsed.destination_ip[i]);
    }
    cout << '\n';

    return match && checksum(serialized) == stored_checksum ? 0 : 1;
}
