#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include <array>

using namespace std;

void serialise_arp_header(
    const arp_header& header,
    array<uint8_t, 28>& output
);

struct arp_header{
    uint16_t hrd_type;
    uint16_t prt_type;
    uint8_t hrd_add_len;
    uint8_t prt_add_len;
    uint16_t oper;
    array<uint8_t,6> sender_hrd_add;
    array<uint8_t,4> sender_prt_add;
    array<uint8_t,6> target_hrd_add;
    array<uint8_t,4> target_prt_add;



};