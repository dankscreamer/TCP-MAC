#include <iostream>
#include <array>
#include <cstdint>
#include <iomanip>

#include "./arp.h"
#include "./ethernet.h"

using namespace std;

void encapsulated_frame(
    const array<uint8_t, 14>& sr_eth,
    const array<uint8_t, 28>& sr_arp,
    array<uint8_t, 42>& ethernet_frame
) {
    for (size_t i = 0; i < sr_eth.size(); i++) {
        ethernet_frame[i] = sr_eth[i];
    }

    for (size_t i = 0; i < sr_arp.size(); i++) {
        ethernet_frame[14 + i] = sr_arp[i];
    }
}

int main() {
    arp_header arp{};

    arp.hrd_type = 0x0001;
    arp.prt_type = 0x0800;
    arp.hrd_add_len = 6;
    arp.prt_add_len = 4;
    arp.oper = 0x0001;
    arp.sender_hrd_add = {10, 10, 10, 10, 10, 10};
    arp.sender_prt_add = {9, 9, 9, 9};
    arp.target_hrd_add = {0, 0, 0, 0, 0, 0};
    arp.target_prt_add = {7, 7, 7, 7};

    array<uint8_t, 28> arp_bytes{};
    serialise_arp_header(arp, arp_bytes);

    arp_header parsed_arp{};
    parse_arp_header(arp_bytes, parsed_arp);

    bool arp_match =
        arp.hrd_type == parsed_arp.hrd_type &&
        arp.prt_type == parsed_arp.prt_type &&
        arp.hrd_add_len == parsed_arp.hrd_add_len &&
        arp.prt_add_len == parsed_arp.prt_add_len &&
        arp.oper == parsed_arp.oper &&
        arp.sender_hrd_add == parsed_arp.sender_hrd_add &&
        arp.sender_prt_add == parsed_arp.sender_prt_add &&
        arp.target_hrd_add == parsed_arp.target_hrd_add &&
        arp.target_prt_add == parsed_arp.target_prt_add;

    cout << (arp_match ? "PASS" : "FAIL") << ": ARP round-trip\n";
    cout << (validate_arp_header(arp) ? "PASS" : "FAIL")
         << ": Valid ARP header\n";

    arp_header invalid_arp = arp;
    invalid_arp.hrd_type = 0x0002;

    cout << (!validate_arp_header(invalid_arp) ? "PASS" : "FAIL")
         << ": Invalid ARP header rejected\n";

    Ethernet_header ethernet{};

    ethernet.destination_mac = {100, 100, 100, 100, 100, 100};
    ethernet.source_mac = {99, 99, 99, 99, 99, 99};
    ethernet.ether_type = 0x0806;

    array<uint8_t, 14> ethernet_bytes{};
    serialise_ethernet_header(ethernet, ethernet_bytes);

    Ethernet_header parsed_ethernet{};
    parse_ethernet(parsed_ethernet, ethernet_bytes);

    bool ethernet_match =
        ethernet.destination_mac == parsed_ethernet.destination_mac &&
        ethernet.source_mac == parsed_ethernet.source_mac &&
        ethernet.ether_type == parsed_ethernet.ether_type;

    cout << (ethernet_match ? "PASS" : "FAIL")
         << ": Ethernet round-trip\n";

    array<uint8_t, 42> ethernet_frame{};
    encapsulated_frame(ethernet_bytes, arp_bytes, ethernet_frame);

    bool correct_length = ethernet_frame.size() == 42;

    bool correct_ethertype =
        ethernet_frame[12] == 0x08 &&
        ethernet_frame[13] == 0x06;

    bool correct_ethernet_payload = true;

    for (size_t i = 0; i < ethernet_bytes.size(); i++) {
        if (ethernet_frame[i] != ethernet_bytes[i]) {
            correct_ethernet_payload = false;
            break;
        }
    }

    bool correct_arp_payload = true;

    for (size_t i = 0; i < arp_bytes.size(); i++) {
        if (ethernet_frame[14 + i] != arp_bytes[i]) {
            correct_arp_payload = false;
            break;
        }
    }

    cout << (correct_length ? "PASS" : "FAIL")
         << ": Frame length is 42 bytes\n";

    cout << (correct_ethertype ? "PASS" : "FAIL")
         << ": EtherType identifies ARP (0x0806)\n";

    cout << (correct_ethernet_payload ? "PASS" : "FAIL")
         << ": Ethernet header copied correctly\n";

    cout << (correct_arp_payload ? "PASS" : "FAIL")
         << ": ARP payload copied correctly\n";

    cout << "\nComplete Ethernet + ARP frame:\n";
    cout << hex << uppercase << setfill('0');

    for (size_t i = 0; i < ethernet_frame.size(); i++) {
        if (i % 8 == 0) {
            cout << "\n" << setw(2) << i << ": ";
        }

        cout << setw(2) << static_cast<int>(ethernet_frame[i]) << " ";
    }

    cout << dec << "\n";

    return 0;
}