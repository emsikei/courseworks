#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <vector>
#include "constants.h"
#include "utilities.h"

/* ================================= Signatures ================================= */

struct PublicKey
{
    t_ullong N;
    t_ullong E;
};

struct PrivateKey
{
    t_ullong D;
    t_ullong N;
};

struct RSAKeys
{
    PublicKey publicKey;
    PrivateKey privateKey;

    RSAKeys(PublicKey publicKey, PrivateKey privateKey)
    {
        this->publicKey = publicKey;
        this->privateKey = privateKey;
    };
};

std::string RSA_Encrypt(std::string &message, const PublicKey &publicKey);
std::string RSA_Decrypt(const std::string &encryptedMessage, const PrivateKey &privateKey);
RSAKeys generateKeys();

/* ================================= Implementation ================================= */

std::string RSA_Encrypt(std::string &message, const PublicKey &publicKey)
{
    std::string encryptedMessage;
    std::vector<t_ullong> M;
    getMessage(message, M);

    for (size_t i = 0; i < M.size(); ++i)
    {
        t_ullong data_to_write = mod(M[i], publicKey.E, publicKey.N);
        encryptedMessage += std::to_string(data_to_write) + " ";
    }

    return base64_encode(encryptedMessage);
}

std::string RSA_Decrypt(const std::string &encryptedMessage, const PrivateKey &privateKey)
{
    std::string decoded = base64_decode(encryptedMessage);

    std::vector<std::string> decryptedMessage = tokenize(decoded);

    convertToNumbers(privateKey.D, privateKey.N, decryptedMessage);

    return formMessage(decryptedMessage);
}

RSAKeys generateKeys()
{
    srand((int)time(0));
    unsigned long randNumber1 = RANGE_MIN + (rand() % (RANGE_MAX - RANGE_MIN + 1));
    unsigned long randNumber2 = RANGE_MIN + (rand() % (RANGE_MAX - RANGE_MIN + 1));

    unsigned long P = getPrime(randNumber1);
    unsigned long Q = getPrime(randNumber2);
    t_ullong N = P * Q;
    t_ullong Phi_N = (P - 1) * (Q - 1);
    t_ullong E = calculate_E(Phi_N);
    t_ullong D = calculate_D(Phi_N, E);

    PublicKey publicKey;
    publicKey.N = N;
    publicKey.E = E;

    PrivateKey privateKey;
    privateKey.D = D;
    privateKey.N = N;

    printKeys(E, N, D, P, Q, Phi_N);

        return RSAKeys(publicKey, privateKey);
}

#endif