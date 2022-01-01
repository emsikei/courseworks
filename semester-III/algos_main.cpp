#include <iostream>
#include <vector>
#include <utility>

#include "headers/aes/aes.h"
#include "headers/rsa/rsa.h"

#include "headers/overloads.h"

int main()
{
    std::string message = "";

    std::cout << "Enter your message: ";
    std::getline(std::cin, message);
    std::cout << std::endl;

    std::string symmetricKey = generateSymmetricKey(16);
    std::string encrypted_AES = AES_Encrypt(message, symmetricKey);
    std::string decrypted_AES = AES_Decrypt(encrypted_AES, symmetricKey);

    std::cout << "Message: " << message << "\n"
              << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << "        AES " << symmetricKey.length() * 8 << "-bit      " << std::endl;
    std::cout << "=========================\n"
              << std::endl;
    std::cout << "Symmetric key: " << symmetricKey << "\n"
              << std::endl;
    std::cout << "Cipher text: " << encrypted_AES << "\n"
              << std::endl;
    std::cout << "Decrypted message: " << decrypted_AES << "\n"
              << std::endl;

    std::cout << "\n\nMessage: " << message << "\n"
              << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << "       RSA 2048-bit      " << std::endl;
    std::cout << "=========================\n"
              << std::endl;

    std::pair<unsigned long long, unsigned long long> publicKey;
    std::pair<unsigned long long, unsigned long long> privateKey;
    generateKey(publicKey, privateKey);

    std::vector<std::string> encrypted_RSA = RSA_Encrypt(message, publicKey);
    std::string decrypted_RSA = RSA_Decrypt(encrypted_RSA, privateKey);

    std::cout << "Cipher text: " << encrypted_RSA << "\n"
              << std::endl;
    std::cout << "Decrypted message: " << decrypted_RSA << "\n"
              << std::endl;
}