# Ethernet Header Serializer and Parser

A C++ implementation of Ethernet II header serialization and parsing, built as the first component of a custom TCP/IP stack.

## Overview

An Ethernet II header is 14 bytes long. It contains the destination MAC address, source MAC address, and EtherType field.

This project implements two operations:

- **Serialization:** Converts an Ethernet header's fields into a 14-byte array.
- **Parsing:** Extracts the fields from a serialized 14-byte Ethernet header.

The goal is to understand how network protocol headers are represented in memory and transmitted as bytes.

## Ethernet II Header Structure

| Field | Size | Byte offsets |
|---|---:|---|
| Destination MAC | 6 bytes | 0–5 |
| Source MAC | 6 bytes | 6–11 |
| EtherType | 2 bytes | 12–13 |
| **Total** | **14 bytes** | **0–13** |

### EtherType values

| Value | Protocol |
|---|---|
| `0x0800` | IPv4 |
| `0x0806` | ARP |
| `0x86DD` | IPv6 |

EtherType uses network byte order (big-endian), meaning the most significant byte is stored first.

For example, `0x0800` is serialized as `08 00`.

## Implementation

### 1. Data structure

The `Ethernet_header` structure, defined in `ethernet.h`, stores:

- `destination_mac`: a six-byte `std::array<uint8_t, 6>`.
- `source_mac`: a six-byte `std::array<uint8_t, 6>`.
- `ether_type`: a `uint16_t` representing the 16-bit protocol identifier.

### 2. Serialization

The `serialise_ethernet_header()` function copies the destination and source MAC addresses into their respective positions in the output array.

It then splits the EtherType into two bytes:

- High byte: `(eth >> 8) & 0xFF`
- Low byte: `eth & 0xFF`

This produces the required 14-byte Ethernet header.

### 3. Parsing

The `parse_ethernet()` function reverses the process:

1. Reads bytes 0–5 into the destination MAC address.
2. Reads bytes 6–11 into the source MAC address.
3. Reconstructs the EtherType from bytes 12 and 13.

The EtherType is reconstructed using a left shift and bitwise OR:

`(byte_12 << 8) | byte_13`

The implementation uses pointers to write the parsed fields into the caller's variables.

## Testing

The current test initializes the header with these values:

- Destination MAC: `64:64:64:64:64:64`
- Source MAC: `63:63:63:63:63:63`
- EtherType: `0x0800` (IPv4)

The program serializes the header, parses the resulting byte array, and prints the serialized bytes and reconstructed fields.

### Expected output

```text
64 64 64 64 64 64 63 63 63 63 63 63 08 00
Destination MAC: 64 64 64 64 64 64
Source MAC: 63 63 63 63 63 63
EtherType: 0x0800
```

This confirms that the expected bytes



# ARP (Address Resolution Protocol) — Serializer and Parser

## Overview

This project implements the serialization and parsing of an Address Resolution Protocol (ARP) message in C++.

ARP is used in IPv4 local area networks to resolve an IPv4 address into the corresponding MAC address. This mapping allows a device to construct an Ethernet frame addressed to the correct device on the local network.

The implementation focuses on constructing a standard 28-byte ARP message, converting its fields into a byte array, and reconstructing the original structure from that byte array.

This is part of a from-scratch TCP/IP stack project, where protocol logic is implemented manually to understand how network protocols represent and exchange data.

## Objectives

- Understand the purpose and structure of ARP.
- Represent an ARP message using a C++ structure.
- Serialize ARP fields into a fixed-size byte array.
- Handle multi-byte integers using network byte order (big-endian).
- Parse a serialized ARP message back into a C++ structure.
- Verify serialization and parsing through round-trip testing.

## What Is ARP?

When a device wants to communicate with another device on the same IPv4 network, it needs the destination device's MAC address to construct an Ethernet frame.

For example, suppose a computer wants to communicate with a device at:

`192.168.1.20`

The computer may know the destination IPv4 address but not its MAC address. It can broadcast an ARP request asking which device owns that IPv4 address.

The device with the matching address responds with an ARP reply containing its MAC address.

The sender can then cache the mapping and use it when constructing Ethernet frames.

### Simplified ARP exchange

1. A host constructs an ARP request.
2. The request identifies the sender's MAC and IPv4 addresses.
3. The request specifies the IPv4 address whose MAC address is being sought.
4. The request is normally broadcast in an Ethernet frame.
5. The target host responds with an ARP reply containing its MAC address.

This project implements the ARP message representation, serialization, and parsing. It does not yet perform this exchange on a real network.

## ARP Message Structure

For Ethernet and IPv4, an ARP message is 28 bytes long.

| Field | Size | Description |
|---|---:|---|
| Hardware type (`hrd_type`) | 2 bytes | Identifies the link-layer hardware type |
| Protocol type (`prt_type`) | 2 bytes | Identifies the network-layer protocol |
| Hardware address length (`hrd_add_len`) | 1 byte | Length of a hardware address |
| Protocol address length (`prt_add_len`) | 1 byte | Length of a protocol address |
| Operation (`oper`) | 2 bytes | Identifies an ARP request or reply |
| Sender hardware address | 6 bytes | Sender's MAC address |
| Sender protocol address | 4 bytes | Sender's IPv4 address |
| Target hardware address | 6 bytes | Target's MAC address |
| Target protocol address | 4 bytes | Target's IPv4 address |
| **Total** | **28 bytes** | |

### Important protocol values

| Field | Value | Meaning |
|---|---|---|
| Hardware type | `0x0001` | Ethernet |
| Protocol type | `0x0800` | IPv4 |
| Hardware address length | `6` | Six-byte MAC address |
| Protocol address length | `4` | Four-byte IPv4 address |
| Operation | `0x0001` | ARP request |
| Operation | `0x0002` | ARP reply |

## Project Structure

The ARP implementation is organized into a header, implementation, and test program.

```text
tcpstck/
├── ethernet.h
├── ethernet.cpp
├── arp.h
├── arp.cpp
└── main.cpp
```

- `arp.h`: Defines the ARP structure and declares the serializer and parser.
- `arp.cpp`: Implements serialization and parsing.
- `main.cpp`: Constructs a test message, serializes it, parses it back, and checks the results.
- `ethernet.h` and `ethernet.cpp`: Contain the previously implemented Ethernet header functionality.

## 1. ARP Data Structure

The `arp_header` structure represents an ARP message in memory.

Its fields include:

- `uint16_t` for 16-bit protocol fields.
- `uint8_t` for single-byte fields.
- `std::array<uint8_t, 6>` for MAC addresses.
- `std::array<uint8_t, 4>` for IPv4 addresses.

The structure provides a convenient way to access the logical fields of an ARP message.

However, the in-memory representation of a C++ structure is not automatically equivalent to the wire format. Compiler padding, alignment, and host byte order can affect how fields are laid out in memory.

For that reason, the implementation explicitly serializes each field into its designated position in the output array.

## 2. Serialization

Serialization converts an `arp_header` object into a 28-byte array.

The serializer has the following interface:

```cpp
void serialise_arp_header(
    const arp_header& header,
    std::array<uint8_t, 28>& output
);
```

### How serialization works

The serializer writes each field to the appropriate byte offsets.

For example, `hrd_type` occupies bytes 0 and 1.

If the value is `0x0001`, the output bytes must be:

```text
00 01
```

This is because protocol fields containing multiple bytes use network byte order, which is big-endian.

For a 16-bit field:

- The high byte is written first.
- The low byte is written second.

The same process is applied to `prt_type` and `oper`.

Single-byte fields can be copied directly. The MAC and IPv4 address arrays are copied into their corresponding ranges in the output buffer.

### Serialization offsets

| Field | Start offset | End offset |
|---|---:|---:|
| `hrd_type` | 0 | 1 |
| `prt_type` | 2 | 3 |
| `hrd_add_len` | 4 | 4 |
| `prt_add_len` | 5 | 5 |
| `oper` | 6 | 7 |
| `sender_hrd_add` | 8 | 13 |
| `sender_prt_add` | 14 | 17 |
| `target_hrd_add` | 18 | 23 |
| `target_prt_add` | 24 | 27 |

The output buffer contains exactly 28 bytes, indexed from 0 to 27.

## 3. Parsing

Parsing performs the reverse operation: it reconstructs an `arp_header` object from a 28-byte array.

The parser has the following interface:

```cpp
void parse_arp_header(
    const std::array<uint8_t, 28>& output,
    arp_header& headers
);
```

### How parsing works

The parser reads each field from its corresponding byte offsets.

For example, the hardware type is reconstructed by taking the first byte, shifting it left by eight bits, and combining it with the second byte using bitwise OR.

Conceptually:

```text
Bytes:                 00 01
First byte shifted:    00 00
Second byte:              01
Reconstructed value:   00 01
```

This reconstructs the original 16-bit value `0x0001`.

The parser applies the same technique to the protocol type and operation fields. It copies the remaining bytes into the appropriate MAC and IPv4 address arrays.

The parser currently assumes that the input is a complete 28-byte ARP message. Protocol validation and handling of unsupported messages are separate future improvements.

## 4. Testing

The test program constructs an ARP message with known values.

### Test input

| Field | Value |
|---|---|
| Hardware type | `0x0001` |
| Protocol type | `0x0800` |
| Hardware address length | `6` |
| Protocol address length | `4` |
| Operation | `0x0001` |
| Sender MAC | `10 10 10 10 10 10` (decimal values) |
| Sender IPv4 bytes | `9 9 9 9` |
| Target MAC | `11 11 11 11 11 11` (decimal values) |
| Target IPv4 bytes | `7 7 7 7` |

These addresses are illustrative test values, not claims about actual network devices.

### Test procedure

1. Create an `arp_header` object with the test values.
2. Serialize the structure into a 28-byte output array.
3. Print the serialized bytes in hexadecimal.
4. Parse the output array into a second `arp_header` object.
5. Compare the original and reconstructed fields.

### Expected serialized output

```text
00: 00 01 08 00 06 04 00 01
08: 0A 0A 0A 0A 0A 0A 09 09
10: 09 09 0B 0B 0B 0B 0B 0B
18: 07 07 07 07
```

The offsets at the beginning of each line are hexadecimal. For example, `18` is hexadecimal 24, the starting offset of the target IPv4 address.

### Round-trip verification

The test compares the original and parsed values for:

- Hardware type.
- Protocol type.
- Hardware address length.
- Protocol address length.
- Operation.
- Sender MAC address.
- Sender IPv4 address.
- Target MAC address.
- Target IPv4 address.

The three 16-bit fields were checked explicitly, and the remaining fields were compared using equality checks. `std::array` supports direct equality comparisons, which compare its elements.

A successful round-trip test shows that the serializer and parser agree for the selected test message.

It does not, by itself, establish that the implementation correctly handles every possible ARP message or transmits packets over a real network.

## 5. Compilation and Execution

Compile the implementation and test program together:

```bash
g++ -std=c++17 main.cpp arp.cpp -o arp_test
```

If the project also includes the Ethernet implementation and `main.cpp` uses it, compile that source file as well:

```bash
g++ -std=c++17 main.cpp ethernet.cpp arp.cpp -o tcpstck
```

Run the resulting executable:

```bash
./arp_test
```

Or, for the combined project:

```bash
./tcpstck
```

## Current Limitations

The current implementation focuses on encoding and decoding ARP messages. It does not yet:

- Validate all incoming ARP field values.
- Reject unsupported hardware or protocol types.
- Distinguish requests from replies beyond reading the operation field.
- Maintain an ARP cache.
- Construct a complete Ethernet frame containing the ARP message.
- Send or receive Ethernet frames on a real network.
- Handle malformed packet input beyond the fixed-size array assumption.

These are separate milestones that can be implemented as the TCP/IP stack develops.

## Next Steps

1. Add ARP protocol validation for hardware type, protocol type, address lengths, and operation.
2. Construct an ARP request with the target hardware address set to zero.
3. Combine the 28-byte ARP message with the 14-byte Ethernet header.
4. Implement or investigate a suitable packet I/O layer for native macOS.
5. Test packet construction independently before attempting real-network transmission.
6. Continue with IPv4 header serialization and parsing.

## Conclusion

This milestone establishes the basic data representation and byte-level conversion of ARP messages.

The implementation demonstrates how a structured C++ object is transformed into a protocol-defined sequence of bytes and reconstructed from that sequence. This is a foundational step toward implementing a TCP/IP stack, where each protocol layer must encode, decode, and validate its own headers.
