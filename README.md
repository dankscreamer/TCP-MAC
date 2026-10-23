# Custom TCP/IP Stack - Ethernet Layering Component

This repository contains a modular, from-scratch implementation of an Ethernet II framing and serialization component written in modern C++. Built as a foundational building block for a custom user-space TCP/IP stack, this module handles the structural layout and byte-level serialization of an Ethernet header into a raw 14-byte network stream.

---

## Project Structure

The codebase is structured following standard header/source separation practices for network protocol layers:

* **`ethernet.h`**: Defines the `Ethernet_header` data structure and declares the prototypes for serialization and parsing routines.
* **`ethernet.cpp` / Driver**: Implements the serialization logic, handles memory layout mapping, and enforces network byte order transformations.

---

## Getting Started

### Prerequisites
* A modern C++ compiler supporting C++11 or higher (e.g., `g++`, `clang++`).

### Compilation & Execution
To compile and run the current implementation via the terminal:

```bash
g++ -std=c++11 main.cpp ethernet.cpp -o tcpip_stack
./tcpip_stack