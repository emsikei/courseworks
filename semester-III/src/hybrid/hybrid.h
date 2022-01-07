#ifndef HYBRID_H
#define HYBRID_H

#include <iostream>
#include <utility>

#include "../aes/aes.h"
#include "../rsa/rsa.h"
#include "../aes/utilities.h"

/* ================================= Signatures ================================= */

std::pair<std::string, std::string> send(const std::string &message,
                                         const PublicKey &publicKey);
std::string receive(const std::string &encryptedMessage,
                    const std::string &encryptedKey,
                    const PrivateKey &privateKey);

/* ================================= Implementation ================================= */

std::pair<std::string, std::string> send(const std::string &message,
                                         const PublicKey &publicKey)
{
    std::string symmetricKey = generateSymmetricKey(16);

    std::string encryptedMessage = AES_Encrypt(message, symmetricKey);

    std::string encryptedKey = RSA_Encrypt(symmetricKey, publicKey);

    return std::make_pair(encryptedMessage, encryptedKey);
}

std::string receive(const std::string &encryptedMessage,
                    const std::string &encryptedKey,
                    const PrivateKey &privateKey)
{
    std::string symmetricKey = RSA_Decrypt(encryptedKey, privateKey);

    std::string message = AES_Decrypt(encryptedMessage, symmetricKey);

    return message;
}

#endif