#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include <array>
using namespace std;


void serialise_ethernet_header(array<uint8_t,6>dest,array<uint8_t,6>sour,uint16_t eth,array<uint8_t,14> *ser);
void parse_ethernet(array<uint8_t,14> *pep,array<uint8_t,6> *R,array<uint8_t,6>*S,uint16_t *E);

struct Ethernet_header{
    array<uint8_t,6> destination_mac;
    array<uint8_t,6> source_mac;
    uint16_t ether_type;

};





