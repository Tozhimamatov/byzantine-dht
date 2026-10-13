# 🌐 Byzantine Fault Tolerant Distributed Hash Table (DHT) in C++20

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A research-grade implementation of a **Distributed Hash Table (DHT)** based on the **Kademlia** algorithm, augmented with **Byzantine Fault Tolerance (BFT)** mechanisms. Built using modern C++20 features for high performance and fault tolerance in decentralized networks.

## 🔬 Scientific & Technical Highlights

- **Kademlia Routing:** Uses XOR metric for distance-based routing and node lookup $O(\log N)$.
- **Byzantine Fault Tolerance (BFT):** Implements redundant storage and quorum-based validation to mitigate malicious or failing nodes.
- **Modern C++20/23:** Utilizes `std::expected`, `std::shared_mutex` for concurrent access, and concepts for template type safety.
- **SHA-256 Identifiers:** Consistent hashing for cryptographic node IDs and data keys.

## 🛠 Build & Run

### Prerequisites
- C++20 compliant compiler (GCC 11+, Clang 13+, MSVC 2019+)
- CMake 3.20+

```bash
mkdir build && cd build
cmake ..
cmake --build .
./dht_demo
