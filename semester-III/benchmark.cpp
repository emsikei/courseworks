// AES-128 & RSA algorithms benchmark

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <chrono>

#include "src/aes/aes.h"
#include "src/rsa/rsa.h"

typedef std::vector<std::string> vec;

std::string getRandomString(const int len);
void createFiles(const std::vector<int> &filesSizes);
void removeFiles(const std::vector<int> &filesSizes);
std::string getFileContent(int fileSize);
void benchmark(const std::vector<int> &filesSizes);
void encryptBenchmark(std::pair<vec, vec> &encryptedMessages,
                      const std::vector<int> &filesSizes,
                      const std::string &symmetricKey,
                      const PublicKey publicKey);
void decryptBenchmark(const std::pair<vec, vec> &encryptedMessages,
                      const std::vector<int> &filesSizes,
                      const std::string &symmetricKey,
                      const PrivateKey privateKey);

int main()
{
    std::vector<int> filesSizes = {32, 64, 128, 256, 512};
    benchmark(filesSizes);
}

// Main benchmark function. Accepts array of requested sizes
void benchmark(const std::vector<int> &filesSizes)
{
    std::cout << "\nCreating files...\n"
              << std::endl;
    createFiles(filesSizes);

    std::string symmetricKey = generateSymmetricKey(16);
    RSAKeys rsaKeys = generateKeys();

    std::pair<vec, vec> encryptedMessages;

    encryptBenchmark(encryptedMessages, filesSizes, symmetricKey, rsaKeys.publicKey);
    decryptBenchmark(encryptedMessages, filesSizes, symmetricKey, rsaKeys.privateKey);

    std::cout << "\nRemoving files..." << std::endl;
    removeFiles(filesSizes);
}

// Benchmark for encryption
void encryptBenchmark(std::pair<vec, vec> &encryptedMessages,
                      const std::vector<int> &filesSizes,
                      const std::string &symmetricKey,
                      const PublicKey publicKey)
{

    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> timeToEncryptAES;
    std::chrono::duration<double> timeToEncryptRSA;

    std::cout << "Encryption:" << std::endl;
    for (size_t i = 0; i < filesSizes.size(); ++i)
    {
        std::string message = getFileContent(filesSizes[i]);

        start = std::chrono::system_clock::now();
        std::string encrypted_AES = AES_Encrypt(message, symmetricKey);
        end = std::chrono::system_clock::now();
        timeToEncryptAES = end - start;
        encryptedMessages.first.push_back(encrypted_AES);

        start = std::chrono::system_clock::now();
        std::string encrypted_RSA = RSA_Encrypt(message, publicKey);
        end = std::chrono::system_clock::now();
        timeToEncryptRSA = end - start;
        encryptedMessages.second.push_back(encrypted_RSA);

        std::cout << "File size - " << filesSizes[i] << "KB: "
                  << " AES - " << timeToEncryptAES.count() << " s\t"
                  << " RSA - " << timeToEncryptRSA.count() << " s" << std::endl;
    }
}

// Benchmark for decryption
void decryptBenchmark(const std::pair<vec, vec> &encryptedMessages,
                      const std::vector<int> &filesSizes,
                      const std::string &symmetricKey,
                      const PrivateKey privateKey)
{

    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> timeToDecryptAES;
    std::chrono::duration<double> timeToDecryptRSA;

    std::cout << "\nDecryption:" << std::endl;
    for (size_t i = 0; i < filesSizes.size(); ++i)
    {
        start = std::chrono::system_clock::now();
        std::string decrypted_AES = AES_Decrypt(encryptedMessages.first[i], symmetricKey);
        end = std::chrono::system_clock::now();
        timeToDecryptAES = end - start;

        start = std::chrono::system_clock::now();
        std::string encrypted_RSA = RSA_Decrypt(encryptedMessages.second[i], privateKey);
        end = std::chrono::system_clock::now();
        timeToDecryptRSA = end - start;

        std::cout << "File size - " << filesSizes[i] << "KB: "
                  << " AES - " << timeToDecryptAES.count() << " s\t"
                  << " RSA - " << timeToDecryptRSA.count() << " s" << std::endl;
    }
}

// Reads file content by its size and returns it
std::string getFileContent(int fileSize)
{
    std::string fileName = "text" + std::to_string(fileSize);
    fileName += "KB.txt";
    std::ifstream infile;
    infile.open(fileName);

    std::string message;
    infile >> message;

    if (!infile)
    {
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
    }

    while (!infile.eof())
    {
        infile >> message;
    }
    infile.close();

    return message;
}

// Creates list of files of given size
void createFiles(const std::vector<int> &filesSizes)
{
    for (size_t i = 0; i < filesSizes.size(); ++i)
    {
        std::string fileName = "text" + std::to_string(filesSizes[i]);
        fileName += "KB.txt";
        std::ofstream ofs(fileName, std::ios::binary | std::ios::out);
        std::string text = getRandomString(filesSizes[i] << 10);
        ofs.write(text.c_str(), text.length());
    }
}

// Removes list of files of given size (used with createFiles())
void removeFiles(const std::vector<int> &filesSizes)
{
    for (size_t i = 0; i < filesSizes.size(); ++i)
    {
        std::string fileName = "text" + std::to_string(filesSizes[i]);
        fileName += "KB.txt";
        [[maybe_unused]] int status = remove(fileName.c_str());
    }
}

// Generates random string 
std::string getRandomString(const int len)
{
    srand(time(0));
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
    {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}