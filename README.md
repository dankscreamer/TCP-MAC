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