#include "Encryption.h"

static const char KEY = 0x5A; // XOR key

std::string Encryption::encrypt(const std::string& plainText) {
    std::string result = plainText;
    for (char& c : result) {
        c ^= KEY;
    }
    return result;
}

std::string Encryption::decrypt(const std::string& cipherText) {
    return encrypt(cipherText); // XOR is symmetric
}

std::string Encryption::hashPassword(const std::string& password) {
    // Simple hash: multiply char codes by position and sum
    unsigned long hash = 5381;
    for (char c : password) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    return std::to_string(hash);
}