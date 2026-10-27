
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include "./packet_dispatcher.h"
#include "./ipv4.h"
#include "./arp.h"
#include "./packet.h"
#include <array>
#include <vector>

using namespace std;

void dispatch(vector<uint8_t> &frame) {

    if (frame.size() < 14) {
        cout << "Ethernet frame too short\n";
        return;
    }

    uint16_t typei = (static_cast<uint16_t>(frame[12]) << 8) |
                     frame[13];

    if (typei == 0x0800) {

        if (frame.size() < 34) {
            cout << "IPv4 header incomplete\n";
            return;
        }

        array<uint8_t, 20> ipv4_bytes{};

        for (int i = 14; i < 34; i++) {
            ipv4_bytes[i - 14] = frame[i];
        }

        ipv4 head{};
        parse_ipv4_header(ipv4_bytes, head);
    }
    else if (typei == 0x0806) {

        if (frame.size() < 42) {
            cout << "ARP header incomplete\n";
            return;
        }

        array<uint8_t, 28> arp_bytes{};

        for (int i = 14; i < 42; i++) {
            arp_bytes[i - 14] = frame[i];
        }

        arp_header arp{};
        parse_arp_header(arp_bytes, arp);
    }
}
