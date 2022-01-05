#include <iostream>
#include <vector>

#include "headers/aes/aes.h"
#include "headers/rsa/rsa.h"

int main()
{
    // std::string message = "An oak tree produces about 10 million acorns during its lifetime.";

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
    std::cout << "        AES 128-bit      " << std::endl;
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
    std::cout << "           RSA           " << std::endl;
    std::cout << "=========================\n"
              << std::endl;

    RSAKeys keys = generateKeys();

    std::string encrypted_RSA = RSA_Encrypt(message, keys.publicKey);
    std::string decrypted_RSA = RSA_Decrypt(encrypted_RSA, keys.privateKey);

    printKeys(keys.publicKey.E, keys.publicKey.N, keys.privateKey.D);
    std::cout << "Cipher text: " << encrypted_RSA << "\n"
              << std::endl;
    std::cout << "Decrypted message: " << decrypted_RSA << "\n"
              << std::endl;
}