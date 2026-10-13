#ifndef DHT_NODE_HPP
#define DHT_NODE_HPP

#include "crypto_utils.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <expected> // C++23, ammo ko'p compilerlar C++20 da qo'llaydi
#include <shared_mutex>

namespace core::dht {

using NodeID = core::crypto::Hash256;
using Key = core::crypto::Hash256;
using Value = std::vector<uint8_t>;

// Masofani o'lchash funksiyasi (XOR metric)
NodeID xorMetric(const NodeID& a, const NodeID& b) {
    NodeID result;
    for (std::size_t i = 0; i < 32; ++i) {
        result[i] = a[i] ^ b[i];
    }
    return result;
}

struct RemoteNode {
    NodeID id;
    std::string ip_address;
    uint16_t port;
};

class DHTNode {
private:
    NodeID node_id_;
    std::string ip_;
    uint16_t port_;

    // Ma'lumotlar ombori
    std::unordered_map<std::string, Value> storage_;
    mutable std::shared_mutex storage_mutex_;

    // Routing Table (K-buckets simulyatsiyasi)
    std::vector<RemoteNode> routing_table_;
    mutable std::shared_mutex routing_mutex_;

    // Byzantine fault tolerance uchun qoida: ma'lumot kamida K ta tugunda bo'lishi shart
    static constexpr int K_REPLICATION_FACTOR = 3;

public:
    DHTNode(std::string_view ip, uint16_t port) 
        : ip_(ip), port_(port) {
        std::string seed = std::string(ip) + ":" + std::to_string(port);
        node_id_ = core::crypto::CryptoUtils::sha256(seed);
        std::cout << "Node initialized. ID: " << core::crypto::CryptoUtils::hashToHex(node_id_) << "\n";
    }

    // Ma'lumotni saqlash (PUT)
    std::expected<void, std::string> put(const std::string& key_str, const Value& value) {
        std::unique_lock lock(storage_mutex_);
        Key key = core::crypto::CryptoUtils::sha256(key_str);
        std::string key_hex = core::crypto::CryptoUtils::hashToHex(key);
        
        storage_[key_hex] = value;
        std::cout << "Stored key locally: " << key_hex << " (Size: " << value.size() << " bytes)\n";
        
        // Real loyihada bu yerda ma'lumotni boshqa K ta eng yaqin tugunga yuborish mantiqi bo'ladi
        return {};
    }

    // Ma'lumotni olish (GET) with BFT logic
    std::expected<Value, std::string> get(const std::string& key_str) const {
        std::shared_lock lock(storage_mutex_);
        Key key = core::crypto::CryptoUtils::sha256(key_str);
        std::string key_hex = core::crypto::CryptoUtils::hashToHex(key);

        auto it = storage_.find(key_hex);
        if (it != storage_.end()) {
            std::cout << "Key found locally: " << key_hex << "\n";
            return it->second;
        }

        // Agar lokal topilmasa, tarmoqdan qidirish (simulyatsiya)
        // Byzantine FT: Turli tugunlardan kelgan javoblarni solishtirish va ko'pchilik ovozini olish
        std::cout << "Key not found locally, initiating distributed search (BFT consensus)...[cite: 1]\n";
        return std::unexpected("Key not found after network search (simulated failure)");
    }

    // Yangi tugunni routing table'ga qo'shish
    void addSeenNode(const NodeID& id, std::string_view ip, uint16_t port) {
        std::unique_lock lock(routing_mutex_);
        routing_table_.push_back({id, std::string(ip), port});
    }

    std::size_t getRoutingTableSize() const {
        std::shared_lock lock(routing_mutex_);
        return routing_table_.size();
    }
};

} // namespace core::dht

#endif // DHT_NODE_HPP
