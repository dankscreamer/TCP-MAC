#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include <array>

using namespace std;




struct ipv4{
    uint8_t version;
    uint8_t ihl;
    uint8_t dscsp_ecn;
    array<uint8_t,2> total_length;
    array<uint8_t,2> identification;
    uint8_t flag;
    array<uint8_t,2> fragment_offset;
    uint8_t ttl;
    uint8_t protocol;
    array<uint8_t,2> header_checksum;
    array<uint8_t,4> source_ip;
    array<uint8_t,4> destination_ip;
};
void serialise_ipv4_header(ipv4 &str,array<uint8_t,20> &ser);
