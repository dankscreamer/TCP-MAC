#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include <array>

using namespace std;

struct tcp{
    uint16_t source_port;
    uint16_t destination_port;
    uint32_t sequence_number;
    uint32_t acknowledgement_number;
    uint16_t data_offset;
    uint8_t reserved_bits;
    uint16_t flags;
    uint16_t window_size;
    uint16_t checksum;
    uint16_t urgent_pointer;



};

void parse_tcp_header(tcp &tl,array<uint8_t,20> &sqs);
void serialise_tcp_header(tcp &tl,array<uint8_t,20> &sms);

