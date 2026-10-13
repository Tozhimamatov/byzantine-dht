#include "../include/dht_node.hpp"
#include "../include/crypto_utils.hpp"
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cout << "===========================================\n";
    std::cout << "   Byzantine Fault Tolerant DHT (C++20)\n";
    std::cout << "===========================================\n\n";

    // 1. Lokal tugunni yaratish
    core::dht::DHTNode my_node("127.0.0.1", 8080);

    // 2. Tarmoqdagi boshqa tugunlarni simulyatsiya qilish
    for (int i = 1; i <= 5; ++i) {
        std::string ip = "192.168.1." + std::to_string(i);
        std::string seed = ip + ":8080";
        auto remote_id = core::crypto::CryptoUtils::sha256(seed);
        my_node.addSeenNode(remote_id, ip, 8080);
    }
    std::cout << "Simulated network with " << my_node.getRoutingTableSize() << " remote nodes.\n\n";

    // 3. Ma'lumotni PUT qilish
    std::string key = "phd_thesis_data";
    std::string data_str = "This is a very important decentralized data block.";
    core::dht::Value data(data_str.begin(), data_str.end());

    std::cout << "--- PUT Operation ---\n";
    auto put_res = my_node.put(key, data);
    if (!put_res) {
        std::cerr << "PUT failed: " << put_res.error() << "\n";
    }
    std::cout << "\n";

    // 4. Ma'lumotni GET qilish
    std::cout << "--- GET Operation ---\n";
    auto get_res = my_node.get(key);
    if (get_res) {
        std::string retrieved_data(get_res->begin(), get_res->end());
        std::cout << "Retrieved Data: " << retrieved_data << "\n";
    } else {
        std::cerr << "GET failed: " << get_res.error() << "\n";
    }

    return 0;
}
