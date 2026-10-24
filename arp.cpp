#include <iostream>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#include "./arp.h"
#include <array>
using namespace std;

void serialise_arp_header(const arp_header &header, array<uint8_t, 28> &output)
{
    output[0] = (header.hrd_type >> 8) & 0xFF;
    output[1] = (header.hrd_type) & 0xFF;
    output[2] = (header.prt_type >> 8) & 0xFF;
    output[3] = (header.prt_type) & 0xFF;
    output[4] = (header.hrd_add_len);
    output[5] = (header.prt_add_len);
    output[6] = (header.oper >> 8) & 0xFF;
    output[7] = (header.oper) & 0xFF;
    for (int i = 8; i <= 13; i++)
    {
        output[i] = header.sender_hrd_add[i - 8];
    }
    for (int i = 14; i <= 17; i++)
    {
        output[i] = header.sender_prt_add[i - 14];
    }
    for (int i = 18; i <= 23; i++)
    {
        output[i] = header.target_hrd_add[i - 18];
    }
    for (int i = 24; i < 28; i++)
    {
        output[i] = header.target_prt_add[i - 24];
    }
}

void parse_arp_header(const array<uint8_t,28> &output, arp_header &headers){
    headers.hrd_type=(output[0]<<8)| output[1];
    headers.prt_type=(output[2]<<8)| output[3];
    headers.hrd_add_len=(output[4]);
    headers.prt_add_len=(output[5]);
    headers.oper=(output[6]<<8) | output[7];
    for (int i=8;i<=13;i++){
        headers.sender_hrd_add[i-8]=output[i];
    }
    for (int i=14;i<=17;i++){
        headers.sender_prt_add[i-14]=output[i];
    }
    for (int i=18;i<=23;i++){
        headers.target_hrd_add[i-18]=output[i];
    }
    for (int i=24;i<=27;i++){
        headers.target_prt_add[i-24]=output[i];
    }
    

     
};

bool validate_arp_header(const arp_header& header) {
    return header.hrd_type == 0x0001 &&
           header.prt_type == 0x0800 &&
           header.hrd_add_len == 6 &&
           header.prt_add_len == 4 &&
           (header.oper == 0x0001 || header.oper == 0x0002);
           
}

    



// int main()
// {
//     array<uint8_t, 28> output{};
//     arp_header parsed{};




//     arp_header arp;
//     arp.hrd_type = 0x1;
//     arp.prt_type = 0x0800;
//     arp.hrd_add_len = 6;
//     arp.prt_add_len = 4;
//     arp.oper = 0x1;
//     arp.sender_hrd_add = {10, 10, 10, 10, 10, 10};
//     arp.sender_prt_add = {9, 9, 9, 9};
//     arp.target_hrd_add = {11, 11, 11, 11, 11, 11};
//     arp.target_prt_add = {7, 7, 7, 7};

//     serialise_arp_header(arp, output);
    

//     cout << hex << uppercase << setfill('0');

//     for (int i = 0; i < 28; i++)
//     {
//         if (i % 8 == 0)
//         {
//             cout << "\n"
//                  << setw(2) << i << ": ";
//         }

//         cout << setw(2) << static_cast<int>(output[i]) << " ";
//     }

//     cout << dec << endl; // testing code ai generated for the arp serialise


//     parse_arp_header(output,parsed);
//     cout << hex << uppercase;
// cout << "Original hardware type: " << arp.hrd_type << endl;
// cout << "Parsed hardware type:   " << parsed.hrd_type << endl;

// cout << "Original protocol type: " << arp.prt_type << endl;
// cout << "Parsed protocol type:   " << parsed.prt_type << endl;

// cout << "Original operation: " << arp.oper << endl;
// cout << "Parsed operation:   " << parsed.oper << endl;//testing code ai generated for arp 



// //round trip test for all the values (ai generated)

// bool match =
//     arp.hrd_add_len == parsed.hrd_add_len &&
//     arp.prt_add_len == parsed.prt_add_len &&
//     arp.sender_hrd_add == parsed.sender_hrd_add &&
//     arp.sender_prt_add == parsed.sender_prt_add &&
//     arp.target_hrd_add == parsed.target_hrd_add &&
//     arp.target_prt_add == parsed.target_prt_add&&arp.hrd_type == parsed.hrd_type &&
//     arp.prt_type == parsed.prt_type &&
//     arp.oper == parsed.oper ;

// cout << (match ? "Remaining fields match" : "Mismatch found") << endl;


// cout << (validate_arp_header(arp) ? "Valid ARP header" : "Unsupported ARP header") << endl;//arp header validation ai generated test

    





// }