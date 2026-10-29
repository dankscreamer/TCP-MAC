
#include "./packet.h"
#include <array>
using namespace std;



void encapsulate_ipv4_packet(
    const std::array<uint8_t, 14>& ethernet_header,
    const std::array<uint8_t, 20>& ipv4_header,
    std::array<uint8_t, 34>& ethernet_frame
    
){
    int i=0;
    
    for (int j=0;j<=1;j++){
        while(i<=13){
            ethernet_frame[i]=ethernet_header[i];
            i++;

        }
        while(i>13 && i<=33){
            ethernet_frame[i]=ipv4_header[i-14];
            i++;

        }
    }
}


