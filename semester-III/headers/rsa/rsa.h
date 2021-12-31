#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <vector>
#include <utility>
#include "constants.h"
#include "utilities.h"

std::vector<std::string> RSA_Encrypt(std::string &message, const std::pair<unsigned long long, unsigned long long> &publicKey)
{
    std::vector<std::string> dataToReturn;
    std::vector<unsigned long long> M;
    getMessage(message, M);

    for (int i = 0; i < M.size(); ++i)
    {
        unsigned long long data_to_write = mod(M[i], publicKey.second, publicKey.first);
        dataToReturn.push_back(std::to_string(data_to_write));
    }

    return dataToReturn;
}

std::string RSA_Decrypt(const std::vector<std::string> &encryptedMessage,
                        const std::pair<unsigned long long, unsigned long long> &privateKey)
{
    std::vector<std::string> decryptedMessage = encryptedMessage;
    unsigned long long D = privateKey.first;
    unsigned long long N = privateKey.second;
    convertToNumbers(D, N, decryptedMessage);

    return formMessage(decryptedMessage);
}

void generateKey(std::pair<unsigned long long, unsigned long long> &publicKey, std::pair<unsigned long long, unsigned long long> &privateKey)
{
    srand((int)time(0));
    unsigned long randNumber1 = RANGE_MIN + (rand() % (RANGE_MAX - RANGE_MIN + 1));
    unsigned long randNumber2 = RANGE_MIN + (rand() % (RANGE_MAX - RANGE_MIN + 1));

    unsigned long P = getPrime(randNumber1);
    unsigned long Q = getPrime(randNumber2);
    unsigned long long N = P * Q;
    unsigned long long Phi_N = (P - 1) * (Q - 1);
    unsigned long long E = calculate_E(Phi_N);
    unsigned long long D = calculate_D(Phi_N, E);

    publicKey.first = N;
    publicKey.second = E;

    privateKey.first = D;
    privateKey.second = N;

    std::cout << "-Public key:\n      • P = " << P << "\n      • Q = " << Q << "\n      • Φ(N) = " << Phi_N << "\n      • D = " << D << "\n-Private key:\n      • N = " << N << "\n      • E = " << E << "\n" << std::endl;
}

#endif