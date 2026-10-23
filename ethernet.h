#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include <array>
using namespace std;


array<uint8_t , 14>serialise_ethernet_header();
void parse_ethernet();

struct Ethernet_header{
    array<uint8_t,6> destination_mac;
    array<uint8_t,6> source_mac;
    uint16_t ether_type;

};





