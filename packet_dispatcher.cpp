
#include <iostream>

#include <iomanip>
#include <algorithm>
#include "./packet_dispatcher.h"
#include "./ipv4.h"
#include "./arp.h"
#include "./packet.h"
#include "./tcp.h"
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

        if (head.ihl != 5) {
            cout << "IPv4 options not supported yet\n";
            return;
        }

        size_t ip_header_length = head.ihl * 4;

        if (head.total_length < ip_header_length) {
            cout << "Invalid IPv4 total length\n";
            return;
        }

        if (frame.size() < 14 + head.total_length) {
            cout << "Incomplete IPv4 packet\n";
            return;
        }

        if (head.protocol == 6) {

            if ((head.flag & 0x1) || head.fragment_offset != 0) {
                cout << "Fragmented IPv4 packet not supported\n";
                return;
            }

            if (head.total_length < ip_header_length + 20) {
                cout << "TCP header incomplete\n";
                return;
            }

            int offset = 14 + (head.ihl * 4);

            array<uint8_t, 20> tcp_bytes{};

            for (int i = offset; i < offset + 20; i++) {
                tcp_bytes[i - offset] = frame[i];
            }

            tcp tp{};
            parse_tcp_header(tp, tcp_bytes);

            if (tp.data_offset < 5) {
                cout << "Invalid TCP data offset\n";
                return;
            }

            size_t tcp_header_length = tp.data_offset * 4;

            if (ip_header_length + tcp_header_length > head.total_length) {
                cout << "TCP header exceeds IPv4 packet length\n";
                return;
            }

            size_t payload_length =
                head.total_length - ip_header_length - tcp_header_length;

            size_t payload_offset = offset + tcp_header_length;

            vector<uint8_t> payload;

            payload.insert(
                payload.end(),
                frame.begin() + payload_offset,
                frame.begin() + payload_offset + payload_length
            );

            cout << "sp" << tp.source_port << '\n';
            cout << "dp" << tp.destination_port << '\n';

            cout << "TCP Header Length: " << tcp_header_length << '\n';
            cout << "Payload Length: " << payload_length << '\n';

            cout << "Payload: ";
            for (uint8_t byte : payload) {
                cout << static_cast<char>(byte);
            }
            cout << '\n';
        }
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
