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
void parse_tcp_header(tcp &tl, array<uint8_t, 20> &sqs) {

    tl.source_port = (static_cast<uint16_t>(sqs[0]) << 8) |
                      sqs[1];

    tl.destination_port = (static_cast<uint16_t>(sqs[2]) << 8) |
                           sqs[3];

    tl.sequence_number = (static_cast<uint32_t>(sqs[4]) << 24) |
                         (static_cast<uint32_t>(sqs[5]) << 16) |
                         (static_cast<uint32_t>(sqs[6]) << 8) |
                         sqs[7];

    tl.acknowledgement_number = (static_cast<uint32_t>(sqs[8]) << 24) |
                                (static_cast<uint32_t>(sqs[9]) << 16) |
                                (static_cast<uint32_t>(sqs[10]) << 8) |
                                sqs[11];

    tl.data_offset = (sqs[12] >> 4) & 0x0F;

    tl.reserved_bits = (sqs[12] >> 1) & 0x07;

    tl.flags = ((sqs[12] & 0x01) << 8) | sqs[13];

    tl.window_size = (static_cast<uint16_t>(sqs[14]) << 8) |
                      sqs[15];

    tl.checksum = (static_cast<uint16_t>(sqs[16]) << 8) |
                   sqs[17];

    tl.urgent_pointer = (static_cast<uint16_t>(sqs[18]) << 8) |
                         sqs[19];
}