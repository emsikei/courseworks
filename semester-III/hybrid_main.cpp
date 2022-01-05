#include <iostream>
#include <vector>
#include <utility>

#include "src/aes/aes.h"
#include "src/rsa/rsa.h"

#include "src/hybrid/hybrid.h"

int main()
{
    // std::string message = "Visual Studio 2019";

    std::string message = "";

    std::cout << "Enter your message to sent: ";
    std::getline(std::cin, message);
    std::cout << std::endl;

    RSAKeys keys = generateKeys();

    std::pair<std::string, std::string> encryptedValues = send(message, keys.publicKey);

    std::string encryptedMessage = encryptedValues.first;
    std::string encryptedKey = encryptedValues.second;

    std::string decryptedMessage = recieve(encryptedMessage, encryptedKey, keys.privateKey);

    std::cout << "Message to send: " << message << "\n"
              << std::endl;

    std::cout << "Encrypted message (message sent): " << encryptedMessage << "\n"
              << std::endl;
    std::cout << "Encrypted symmetric key (key sent): " << encryptedKey << "\n"
              << std::endl;

    std::cout << "Recieved message: " << decryptedMessage << "\n"
              << std::endl;
}