
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include "./ipv4.h"
#include <array>

using namespace std;

void serialise_ipv4_header(const ipv4& str, array<uint8_t, 20>& rst) {
    rst[0] = (str.version & 0x0F) << 4 | (str.ihl & 0x0F);
    rst[1] = str.dscsp_ecn;
    rst[2] = (str.total_length >> 8) & 0xFF;
    rst[3] = str.total_length & 0xFF;
    rst[4] = (str.identification >> 8) & 0xFF;
    rst[5] = str.identification & 0xFF;

    uint16_t flags_offset =
        ((str.flag & 0x07) << 13) |
        (str.fragment_offset & 0x1FFF);

    rst[6] = (flags_offset >> 8) & 0xFF;
    rst[7] = flags_offset & 0xFF;
    rst[8] = str.ttl;
    rst[9] = str.protocol;
    rst[10] = 0;
    rst[11] = 0;

    for (int i = 12; i <= 15; i++) {
        rst[i] = str.source_ip[i - 12];
    }

    for (int i = 16; i <= 19; i++) {
        rst[i] = str.destination_ip[i - 16];
    }

    uint16_t chk = checksum(rst);
    rst[10] = (chk >> 8) & 0xFF;
    rst[11] = chk & 0xFF;
}

void parse_ipv4_header(const array<uint8_t, 20>& serial, ipv4& pho) {
    pho.version = (serial[0] >> 4) & 0x0F;
    pho.ihl = serial[0] & 0x0F;
    pho.dscsp_ecn = serial[1];

    pho.total_length =
        ((serial[2]) << 8) | serial[3];

    pho.identification =
        ((serial[4]) << 8) | serial[5];

    pho.flag = (serial[6] >> 5) & 0x07;

    pho.fragment_offset =
        ((serial[6] & 0x1F) << 8) | serial[7];

    pho.ttl = serial[8];
    pho.protocol = serial[9];

    pho.header_checksum =
        ((serial[10]) << 8) | serial[11];

    for (int i = 12; i <= 15; i++) {
        pho.source_ip[i - 12] = serial[i];
    }

    for (int i = 16; i <= 19; i++) {
        pho.destination_ip[i - 16] = serial[i];
    }
}

uint16_t checksum(array<uint8_t, 20> header) {
    header[10] = 0;
    header[11] = 0;

    uint32_t sum = 0;

    for (int i = 0; i < 20; i += 2) {
        uint16_t word =
            ((header[i]) << 8) |
            header[i + 1];

        sum += word;
    }

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return static_cast<uint16_t>(~sum);
}
