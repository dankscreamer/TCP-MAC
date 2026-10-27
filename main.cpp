
#include <iostream>
#include <vector>
#include <array>
#include <cstdint>

#include "./packet_dispatcher.h"
#include "./ethernet.h"
#include "./ipv4.h"
#include "./arp.h"

using namespace std;

int main() {

    // ==========================================
    // TEST 1: IPv4 FRAME
    // ==========================================

    cout << "===== Testing IPv4 Dispatch =====\n";

    Ethernet_header eth{};
    eth.destination_mac = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    eth.source_mac      = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    eth.ether_type      = 0x0800;

    array<uint8_t, 14> eth_bytes{};
    serialise_ethernet_header(eth, eth_bytes);

    ipv4 ip{};
    ip.version          = 4;
    ip.ihl              = 5;
    ip.dscsp_ecn        = 0;
    ip.total_length     = 20;
    ip.identification   = 0x1234;
    ip.flag             = 2;
    ip.fragment_offset  = 0;
    ip.ttl              = 64;
    ip.protocol         = 6;
    ip.source_ip        = {192, 168, 1, 10};
    ip.destination_ip   = {192, 168, 1, 20};

    array<uint8_t, 20> ip_bytes{};
    serialise_ipv4_header(ip, ip_bytes);

    vector<uint8_t> ipv4_frame;

    for (int i = 0; i < 14; i++)
        ipv4_frame.push_back(eth_bytes[i]);

    for (int i = 0; i < 20; i++)
        ipv4_frame.push_back(ip_bytes[i]);

    cout << "Frame size: " << ipv4_frame.size() << '\n';
    dispatch(ipv4_frame);


    // ==========================================
    // TEST 2: ARP FRAME
    // ==========================================

    cout << "\n===== Testing ARP Dispatch =====\n";

    eth.ether_type = 0x0806;
    serialise_ethernet_header(eth, eth_bytes);

    arp_header arp{};
    arp.hrd_type     = 0x0001;
    arp.prt_type     = 0x0800;
    arp.hrd_add_len  = 6;
    arp.prt_add_len  = 4;
    arp.oper         = 0x0001;

    arp.sender_hrd_add = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    arp.sender_prt_add = {192, 168, 1, 10};
    arp.target_hrd_add = {0, 0, 0, 0, 0, 0};
    arp.target_prt_add = {192, 168, 1, 20};

    array<uint8_t, 28> arp_bytes{};
    serialise_arp_header(arp, arp_bytes);

    vector<uint8_t> arp_frame;

    for (int i = 0; i < 14; i++)
        arp_frame.push_back(eth_bytes[i]);

    for (int i = 0; i < 28; i++)
        arp_frame.push_back(arp_bytes[i]);

    cout << "Frame size: " << arp_frame.size() << '\n';
    dispatch(arp_frame);

    return 0;
}
