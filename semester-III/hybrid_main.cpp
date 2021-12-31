#include <iostream>
#include <vector>
#include <utility>

#include "headers/aes/aes.h"
#include "headers/rsa/rsa.h"

#include "headers/hybrid/hybrid.h"

std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &values)
{
    if (!os.good())
        return os;

    for (auto value : values)
    {
        os << value << " ";
    }
    return os;
}

int main()
{
    std::string message = "Visual Studio 2019";

    std::pair<unsigned long long, unsigned long long> publicKey;
    std::pair<unsigned long long, unsigned long long> privateKey;
    generateKey(publicKey, privateKey);

    std::pair<std::string, std::vector<std::string>> encryptedValues = send(message, publicKey);

    std::string encryptedMessage = encryptedValues.first;
    std::vector<std::string> encryptedKey = encryptedValues.second;

    std::string decryptedMessage = recieve(encryptedMessage, encryptedKey, privateKey);

    std::cout << "Message to send: " << message << "\n"
              << std::endl;

    std::cout << "Encrypted message (message sent): " << encryptedMessage << "\n"
              << std::endl;
    std::cout << "Encrypted symmetric key (key sent): " << encryptedKey << "\n"
              << std::endl;

    std::cout << "Recieved message: " << decryptedMessage << "\n"
              << std::endl;


}