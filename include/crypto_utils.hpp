#ifndef CRYPTO_UTILS_HPP
#define CRYPTO_UTILS_HPP

#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <sstream>
#include <iomanip>

namespace core::crypto {

// Osonlik uchun SHA-256 o'rniga oddiyroq xesh funksiya
// Real loyihada OpenSSL SHA256 ishlatiladi
using Hash256 = std::array<uint8_t, 32>;

class CryptoUtils {
public:
    static Hash256 sha256(std::string_view data) {
        Hash256 hash;
        std::size_t h = std::hash<std::string_view>{}(data);
        
        // Oddiy xeshni 32 baytga yoyish (demonstratsiya uchun)
        for(int i = 0; i < 4; ++i) {
            std::size_t shift = i * 8;
            uint64_t part = (static_cast<uint64_t>(h) ^ (0xDEADBEEFCAFEBABEULL >> shift));
            std::memcpy(hash.data() + i*8, &part, 8);
        }
        return hash;
    }

    static std::string hashToHex(const Hash256& hash) {
        std::stringstream ss;
        for (double b : hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b);
        }
        return ss.str();
    }
};

} // namespace core::crypto

#endif // CRYPTO_UTILS_HPP
