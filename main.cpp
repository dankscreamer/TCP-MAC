
#include <iostream>
#include <array>
#include <vector>
#include <cstdint>

#include "./ethernet.h"
#include "./ipv4.h"
#include "./tcp.h"
#include "./packet_dispatcher.h"

using namespace std;

int main() {

    Ethernet_header eth{};

    eth.destination_mac = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    eth.source_mac = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    eth.ether_type = 0x0800;

    array<uint8_t, 14> eth_bytes{};
    serialise_ethernet_header(eth, eth_bytes);


    ipv4 ip{};

    ip.version = 4;
    ip.ihl = 5;
    ip.dscsp_ecn = 0;
    ip.total_length = 40;
    ip.identification = 0x1234;
    ip.flag = 2;
    ip.fragment_offset = 0;
    ip.ttl = 64;
    ip.protocol = 6;
    ip.source_ip = {192, 168, 1, 10};
    ip.destination_ip = {192, 168, 1, 20};

    array<uint8_t, 20> ip_bytes{};
    serialise_ipv4_header(ip, ip_bytes);


    tcp tp{};

    tp.source_port = 8080;
    tp.destination_port = 443;
    tp.sequence_number = 0x12345678;
    tp.acknowledgement_number = 0xABCDEF12;
    tp.data_offset = 5;
    tp.reserved_bits = 0;
    tp.flags = 0x010;
    tp.window_size = 65535;
    tp.checksum = 0x1234;
    tp.urgent_pointer = 0;

    array<uint8_t, 20> tcp_bytes{};
    serialise_tcp_header(tp, tcp_bytes);


    vector<uint8_t> frame;

    for (int i = 0; i < 14; i++) {
        frame.push_back(eth_bytes[i]);
    }

    for (int i = 0; i < 20; i++) {
        frame.push_back(ip_bytes[i]);
    }

    for (int i = 0; i < 20; i++) {
        frame.push_back(tcp_bytes[i]);
    }


    cout << "Complete frame size: " << frame.size() << " bytes\n";

    cout << "\nDispatching frame...\n";

    dispatch(frame);

    return 0;
}
