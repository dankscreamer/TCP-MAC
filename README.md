#TCP-IP implementation on mac os 
This project is implementation and a way to understand how does ARP/IPV4/Ethernet connections work bare bones and also how is tcp connection established between two edge devices 

Common things in each of the header files 
Parsing 
Serialise 
These two functions are common for all the transmission types.

Implementation roadmap->
ethernet.cpp-> the structure of ethernet header is defined by
6 bytes for destination mac 
6 bytes for source mac
2 bytes for ether type 
##The ether type defines how to and which transmission type to use be it ARP/IPV4 


 
