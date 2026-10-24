#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include "./ethernet.h"
#include <array>
using namespace std;
void parse_ethernet(array<uint8_t,14> *pep,array<uint8_t,6> *R,array<uint8_t,6>*S,uint16_t *E){

    for (int i=0;i<=5;i++){
        (*R)[i]=(*pep)[i];

    }
    for(int i=6;i<=11;i++){
        (*S)[i-6]=(*pep)[i];

    }
    (*E)=((*pep)[12]<<8 |(*pep)[13]);
    


}
void serialise_ethernet_header(array<uint8_t,6>dest,array<uint8_t,6>sour,uint16_t eth,array<uint8_t,14> *ser){


    for (int i=0;i<=5;i++){
        (*ser)[i]=dest[i];
    }
    for (int i=6;i<=11;i++){
        (*ser)[i]=sour[i-6];
    }
    (*ser)[12] = (eth >> 8) & 0xFF;
    (*ser)[13] = eth & 0xFF;

} 

int main(){
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