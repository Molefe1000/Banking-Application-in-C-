#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

class Encryption {
public:
    // Simple XOR-based encryption with a key
    static std::string encrypt(const std::string& plainText);
    static std::string decrypt(const std::string& cipherText);
    
    // Hash for passwords (simple hash)
    static std::string hashPassword(const std::string& password);
};

#endif