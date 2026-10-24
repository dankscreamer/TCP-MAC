#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include "./arp.h"
#include <array>
using namespace std;


void serialise_arp_header(const arp_header& header,array<uint8_t, 28>& output){
    output[0]=(header.hrd_type >> 8) & 0xFF;
    output[1]=(header.hrd_type ) & 0xFF;
    output[2]=(header.prt_type >> 8) & 0xFF;
    output[3]=(header.prt_type ) & 0xFF;
    output[4]=(header.hrd_add_len);
    output[5]=(header.prt_add_len);
    output[6]=(header.oper >> 8) & 0xFF;
    output[7]=(header.oper)& 0xFF;
    for (int i=8;i<=13;i++){
        output[i]=header.sender_hrd_add[i-8];
    }
    for(int i=14;i<=17;i++){
        output[i]=header.sender_prt_add[i-14];

    }
    for(int i=18;i<=23;i++){
        output[i]=header.target_hrd_add[i-18];
    }
    for(int i=24;i<28;i++){
        output[i]=header.target_prt_add[i-24];
    }



    
}


int main(){
    array<uint8_t,28> output;

    arp_header arp;
    arp.hrd_type=0x1;
    arp.prt_type=0x0800;
    arp.hrd_add_len=6;
    arp.prt_add_len=4;
    arp.oper=0x1;
    arp.sender_hrd_add={10,10,10,10,10,10};
    arp.sender_prt_add={9,9,9,9};
    arp.target_hrd_add={11,11,11,11,11,11};
    arp.target_prt_add={7,7,7,7};


    serialise_arp_header(arp,output);



    cout << hex << uppercase << setfill('0');

    for (int i = 0; i < 28; i++) {
        if (i % 8 == 0) {
            cout << "\n" << setw(2) << i << ": ";
        }

        cout << setw(2) << static_cast<int>(output[i]) << " ";
    }

    cout << dec << endl;    //testing code ai generated

}