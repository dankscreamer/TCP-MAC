
#include <array>
#include <cstdint>

void encapsulate_ipv4_packet(
    const std::array<uint8_t, 14>& ethernet_header,
    const std::array<uint8_t, 20>& ipv4_header,
    std::array<uint8_t, 34>& ethernet_frame
);


