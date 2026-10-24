#include <iostream>
#include <array>
#include <cstdint>
#include <iomanip>

#include "./arp.h"
#include "./ethernet.h"


using namespace std;

int main() {    array<uint8_t, 28> output{};
    arp_header parsed{};




    arp_header arp;
    arp.hrd_type = 0x1;
    arp.prt_type = 0x0800;
    arp.hrd_add_len = 6;
    arp.prt_add_len = 4;
    arp.oper = 0x1;
    arp.sender_hrd_add = {10, 10, 10, 10, 10, 10};
    arp.sender_prt_add = {9, 9, 9, 9};
    arp.target_hrd_add = {11, 11, 11, 11, 11, 11};
    arp.target_prt_add = {7, 7, 7, 7};

    serialise_arp_header(arp, output);
    

    cout << hex << uppercase << setfill('0');

    for (int i = 0; i < 28; i++)
    {
        if (i % 8 == 0)
        {
            cout << "\n"
                 << setw(2) << i << ": ";
        }

        cout << setw(2) << static_cast<int>(output[i]) << " ";
    }

    cout << dec << endl; // testing code ai generated for the arp serialise


    parse_arp_header(output,parsed);
    cout << hex << uppercase;
cout << "Original hardware type: " << arp.hrd_type << endl;
cout << "Parsed hardware type:   " << parsed.hrd_type << endl;

cout << "Original protocol type: " << arp.prt_type << endl;
cout << "Parsed protocol type:   " << parsed.prt_type << endl;

cout << "Original operation: " << arp.oper << endl;
cout << "Parsed operation:   " << parsed.oper << endl;//testing code ai generated for arp 



//round trip test for all the values (ai generated)

bool match =
    arp.hrd_add_len == parsed.hrd_add_len &&
    arp.prt_add_len == parsed.prt_add_len &&
    arp.sender_hrd_add == parsed.sender_hrd_add &&
    arp.sender_prt_add == parsed.sender_prt_add &&
    arp.target_hrd_add == parsed.target_hrd_add &&
    arp.target_prt_add == parsed.target_prt_add&&arp.hrd_type == parsed.hrd_type &&
    arp.prt_type == parsed.prt_type &&
    arp.oper == parsed.oper ;

cout << (match ? "Remaining fields match" : "Mismatch found") << endl;


cout << (validate_arp_header(arp) ? "Valid ARP header" : "Unsupported ARP header") << endl;//arp header validation ai generated test



//Ethernet
Ethernet_header header; 
    array<uint8_t,14> serialised_ethernet_header;
    array<uint8_t, 14> *ptr= &serialised_ethernet_header;

    header.destination_mac={100,100,100,100,100,100};
    header.source_mac={99,99,99,99,99,99};
    header.ether_type=0x0800;
    array<uint8_t,6>destination_eth=header.destination_mac;
    array<uint8_t,6>source_eth=header.source_mac;
    uint16_t ether_type_eth=header.ether_type;



    array<uint8_t,6>R_destination_mac;
    array<uint8_t,6>R_source_mac;
    uint16_t R_ether_type;
    



    serialise_ethernet_header(destination_eth,source_eth,ether_type_eth,ptr);
    parse_ethernet(&serialised_ethernet_header,&R_destination_mac,&R_source_mac,&R_ether_type);
    cout << hex << setfill('0'); // testing code ai generated

    for (int i = 0; i < 14; i++) {
    cout << setw(2) << static_cast<int>(serialised_ethernet_header[i]) << " ";
    }

    cout << endl;
    cout << hex << setfill('0');

    cout << "Destination MAC: ";
    for (int i = 0; i < 6; i++) {
        cout << setw(2) << static_cast<int>(R_destination_mac[i]) << " ";
    }

    cout << "\nSource MAC: ";
    for (int i = 0; i < 6; i++) {
        cout << setw(2) << static_cast<int>(R_source_mac[i]) << " ";
    }

    cout << "\nEtherType: 0x"
        << setw(4) << R_ether_type << endl;

    


}