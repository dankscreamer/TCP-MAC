#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include "./ethernet.h"
#include <array>
using namespace std;
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include <array>
using namespace std;



void serialise_ethernet_header(Ethernet_header &head,
                               array<uint8_t, 14> &serialised);

void parse_ethernet(Ethernet_header &head,
                    array<uint8_t, 14> &parsed);

void parse_ethernet(Ethernet_header &head,
                    array<uint8_t, 14> &parsed) {

    for (int i = 0; i < 6; i++) {
        head.destination_mac[i] = parsed[i];
    }

    for (int i = 6; i < 12; i++) {
        head.source_mac[i - 6] = parsed[i];
    }

    head.ether_type =
        (static_cast<uint16_t>(parsed[12]) << 8) |
        parsed[13];
}

void serialise_ethernet_header(Ethernet_header &head,
                               array<uint8_t, 14> &serialised) {

    for (int i = 0; i < 6; i++) {
        serialised[i] = head.destination_mac[i];
    }

    for (int i = 6; i < 12; i++) {
        serialised[i] = head.source_mac[i - 6];
    }

    serialised[12] = (head.ether_type >> 8) & 0xFF;
    serialised[13] = head.ether_type & 0xFF;
}

// int main(){
//     Ethernet_header header; 
//     array<uint8_t,14> serialised_ethernet_header;
//     array<uint8_t, 14> *ptr= &serialised_ethernet_header;

//     header.destination_mac={100,100,100,100,100,100};
//     header.source_mac={99,99,99,99,99,99};
//     header.ether_type=0x0800;
//     array<uint8_t,6>destination_eth=header.destination_mac;
//     array<uint8_t,6>source_eth=header.source_mac;
//     uint16_t ether_type_eth=header.ether_type;



//     array<uint8_t,6>R_destination_mac;
//     array<uint8_t,6>R_source_mac;
//     uint16_t R_ether_type;
    



//     serialise_ethernet_header(destination_eth,source_eth,ether_type_eth,ptr);
//     parse_ethernet(&serialised_ethernet_header,&R_destination_mac,&R_source_mac,&R_ether_type);
//     cout << hex << setfill('0'); // testing code ai generated

//     for (int i = 0; i < 14; i++) {
//     cout << setw(2) << static_cast<int>(serialised_ethernet_header[i]) << " ";
//     }

//     cout << endl;
//     cout << hex << setfill('0');

//     cout << "Destination MAC: ";
//     for (int i = 0; i < 6; i++) {
//         cout << setw(2) << static_cast<int>(R_destination_mac[i]) << " ";
//     }

//     cout << "\nSource MAC: ";
//     for (int i = 0; i < 6; i++) {
//         cout << setw(2) << static_cast<int>(R_source_mac[i]) << " ";
//     }

//     cout << "\nEtherType: 0x"
//         << setw(4) << R_ether_type << endl;

    


// }