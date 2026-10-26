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
    uint16_t total_length;
    uint16_t  identification;
    uint8_t flag;
    uint16_t fragment_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t header_checksum;
    array<uint8_t,4> source_ip;
    array<uint8_t,4> destination_ip;
};
void serialise_ipv4_header(const ipv4 &str,array<uint8_t,20> &ser);
void parse_ipv4_header(const array<uint8_t,20> &serial, ipv4 &pho);
uint16_t checksum(array<uint8_t,20> header);


