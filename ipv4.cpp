#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include "./ipv4.h"

#include <array>
using namespace std;

void serialise_ipv4_header(ipv4 &str,array<uint8_t,20> &rst){
    
    
    rst[0]= (str.version & 0x0F) << 4 | (str.ihl & 0x0F) ;
    rst[1]=str.dscsp_ecn;
    rst[2]=(str.total_length  >>8) & 0x0FF ;
    rst[3]=(str.total_length & 0xFF );
    rst[4]=(str.identification>>8)& 0x0FF;
    rst[5]=(str.identification & 0x0FF);
    uint16_t flags_offset =
    ((str.flag & 0x07) << 13) |
    (str.fragment_offset & 0x1FFF);

    rst[6] = (flags_offset >> 8) & 0xFF;
    rst[7] = flags_offset & 0xFF;
    rst[8]=str.ttl;
    rst[9]=str.protocol;
    rst[10]=(str.header_checksum >> 8 )& 0XFF;
    rst[11]=(str.header_checksum & 0xFF);
    for (int i = 12;i<=15;i++){
        rst[i]=str.source_ip[i-12];
    }
    for (int i = 16; i<=19;i++){
        rst[i]=str.destination_ip[i-16];
    }





}