#include <iostream>

#include <iomanip>
#include <algorithm>
#include "./tcp.h"
#include <array>

using namespace std;

void serialise_tcp_header(tcp &tl,array<uint8_t,20> &sms){
    sms[1]=tl.source_port & 0xFF;
    sms[0]=(tl.source_port >> 8) &0xff;
    sms[2]=(tl.destination_port >> 8) &0xff;
    sms[3]=tl.destination_port & 0Xff;
    sms[4]=(tl.sequence_number >> 24) & 0xff;
    sms[5]=(tl.sequence_number >> 16) & 0xff;
    sms[6]=(tl.sequence_number >> 8 )& 0xff;
    sms[7]=(tl.sequence_number ) & 0xff;
    sms[8]=(tl.acknowledgement_number >> 24) & 0xff;
    sms[9]=(tl.acknowledgement_number >> 16) & 0xff;
    sms[10]=(tl.acknowledgement_number>> 8 )& 0xff;
    sms[11]=(tl.acknowledgement_number ) & 0xff;
    
    sms[12] = ((tl.data_offset & 0x0F) << 4) | ((tl.reserved_bits & 0x07) << 1) | ((tl.flags >> 8) & 0x01);
    sms[13] = tl.flags & 0xFF;

    sms[14] = (tl.window_size >> 8) & 0xFF;
    sms[15] = tl.window_size & 0xFF;

    sms[16] = (tl.checksum >> 8) & 0xFF;
    sms[17] = tl.checksum & 0xFF;

    sms[18] = (tl.urgent_pointer >> 8) & 0xFF;
    sms[19] = tl.urgent_pointer & 0xFF;
    




 
    

}