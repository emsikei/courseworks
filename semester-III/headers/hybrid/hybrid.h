#ifndef HYBRID_H
#define HYBRID_H

#include <iostream>
#include <vector>
#include <utility>

#include "../aes/aes.h"
#include "../rsa/rsa.h"
#include "../aes/utilities.h"

std::pair<std::string, std::vector<std::string>> send(const std::string &message,
                                                      const std::pair<unsigned long long, unsigned long long> &publicKey)
{
    std::string symmetricKey = generateSymmetricKey(16);

    std::string encryptedMessage = AES_Encrypt(message, symmetricKey);

    std::vector<std::string> encryptedKey = RSA_Encrypt(symmetricKey, publicKey);

    return std::make_pair(encryptedMessage, encryptedKey);
}

std::string recieve(const std::string &encryptedMessage,
                    const std::vector<std::string> &encryptedKey,
                    const std::pair<unsigned long long, unsigned long long> &privateKey)
{
    std::string symmetricKey = RSA_Decrypt(encryptedKey, privateKey);

    std::string message = AES_Decrypt(encryptedMessage, symmetricKey);

    return message;
}

#endif