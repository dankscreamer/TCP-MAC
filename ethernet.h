#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include <array>
using namespace std;

struct Ethernet_header{
    array<uint8_t,6> destination_mac;
    array<uint8_t,6> source_mac;
    uint16_t ether_type;

};


void serialise_ethernet_header(Ethernet_header &head,array<uint8_t,14> &serialised);
void parse_ethernet(Ethernet_header &head,array<uint8_t,14> &parsed );







