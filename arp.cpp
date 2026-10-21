
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>

using namespace std;

int main() {
    uint8_t syn_arp[28] = {
       0, 1,                          // Hardware type: Ethernet
    8, 0,                          // Protocol type: IPv4
    6,                             // Hardware address length
    4,                             // Protocol address length
    0, 1,                          // Operation: ARP request
    2, 0, 0, 0, 0, 1,             // Sender MAC
    192, 168, 1, 10,               // Sender IP: 192.168.1.10
    0, 0, 0, 0, 0, 0,             // Target MAC: unknown
    192, 168, 1, 20             // EtherType: ARP
    };





}
