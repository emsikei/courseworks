#ifndef RSA_UTILITIES_H
#define RSA_UTILITIES_H

#include <iostream>
#include <vector>

typedef unsigned long long int t_ullong;
typedef unsigned char uchar;

static const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* ================================= Signatures ================================= */

t_ullong mod(t_ullong a, t_ullong b, t_ullong c);
std::string formMessage(std::vector<std::string> &M);
bool isPrime(int n);
int getPrime(int N);
int gcd(int a, int b);
int calculate_E(t_ullong Phi_N);
int calculate_D(t_ullong Phi_N, t_ullong E);

void getMessage(std::string &message, std::vector<t_ullong> &M);
void convertToNumbers(t_ullong D, t_ullong N, std::vector<std::string> &M);

void printKeys(t_ullong E, t_ullong N, t_ullong D, t_ullong P, t_ullong Q, t_ullong phi);
void print_hex(char *msg, int len);

std::string base64_encode(const std::string &in);
std::string base64_decode(const std::string &in);

std::vector<std::string> tokenize(const std::string &str);

/* ================================= Implementation ================================= */

t_ullong mod(t_ullong a, t_ullong b, t_ullong c)
{
    if (b == 0)
    {
        return 1;
    }
    else if (b % 2 == 0)
    {
        t_ullong d = mod(a, b / 2, c);
        return (d * d) % c;
    }
    else
    {
        return ((a % c) * mod(a, b - 1, c)) % c;
    }
}

void getMessage(std::string &message, std::vector<t_ullong> &M)
{
    std::string two_numbers = "";
    for (size_t i = 0; i < message.length(); ++i)
    {
        if (i % 2 == 0 && i != 0)
        {
            M.push_back(std::stoll(two_numbers));
            two_numbers = "";
        }
        std::string newNumber = std::to_string((int)message[i] - 31);
        if (newNumber.length() == 1 && i % 2 != 0)
        {
            newNumber.insert(newNumber.begin(), '0');
        }
        two_numbers += newNumber;
    }
    if (message.length() % 2 != 0)
    {
        two_numbers += "00";
    }
    M.push_back(std::stoll(two_numbers));
    two_numbers.clear();
}

void convertToNumbers(t_ullong D, t_ullong N, std::vector<std::string> &M)
{
    std::vector<t_ullong> C;
    for (size_t i = 0; i < M.size() - 1; ++i)
    {
        C.push_back(std::stoll(M[i]));
    }

    M.clear();
    for (size_t i = 0; i < C.size(); ++i)
    {
        M.push_back(std::to_string(mod(C[i], D, N)));
    }
}

std::string formMessage(std::vector<std::string> &M)
{
    std::string message;
    for (size_t i = 0; i < M.size(); ++i)
    {
        if (M[i].length() == 4)
        {
            std::string number = "";
            number += M[i][0];
            number += M[i][1];
            message += (char)(std::stod(number) + 31);
            number = "";
            number += M[i][2];
            number += M[i][3];
            message += (char)(std::stod(number) + 31);
        }
        else
        {
            std::string number = "";
            number += M[i][0];
            message += (char)(std::stod(number) + 31);
            number = "";
            number += M[i][1];
            number += M[i][2];
            message += (char)(std::stod(number) + 31);
        }
    }

    return message;
}

bool isPrime(int n)
{
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;

    for (int i = 5; i * i <= n; i = i + 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
        {
            return false;
        }
    }
    return true;
}

int getPrime(int N)
{
    if (N <= 1)
    {
        return 2;
    }

    int prime = N;
    bool found = false;

    while (!found)
    {
        prime++;
        if (isPrime(prime))
        {
            found = true;
        }
    }

    return prime;
}

int gcd(int a, int b)
{
    if (b == 0)
    {
        return a;
    }
    return gcd(b, a % b);
}

int calculate_E(t_ullong Phi_N)
{
    size_t i = 2;
    while (gcd(i, Phi_N) != 1)
    {
        i++;
    }
    if (i >= Phi_N)
    {
        exit(-1);
    }
    else
    {
        return i;
    }
}

int calculate_D(t_ullong Phi_N, t_ullong E)
{
    int k = 0;
    while (((k * Phi_N) + 1) % E != 0)
    {
        k++;
    }
    return (((k * Phi_N) + 1) / E);
}

void print_hex(char *msg, int len)
{
    std::cout << "Your message in hex: ";
    for (int i = 0; i < len; i++)
    {
        printf("%02X ", (unsigned char)*(msg + i));
    }
    std::cout << std::endl;
}

void printKeys(t_ullong E, t_ullong N, t_ullong D, t_ullong P, t_ullong Q, t_ullong phi)
{
    std::cout << "-Public key:\n      •P = " << P << "\n      •Q = " << Q << "\n      •Φ(N) = " << phi << "\n      •D = " << D << "\n   -Private key:\n      •N = " << N << "\n      •E = " << E << std::endl;
}

std::string base64_encode(const std::string &in)
{
    std::string out;

    int val = 0, valb = -6;
    for (uchar c : in)
    {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            out.push_back(alphabet[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
        out.push_back(alphabet[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4)
        out.push_back('=');
    return out;
}

std::string base64_decode(const std::string &in)
{
    std::string out;

    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
        T[alphabet[i]] = i;

    int val = 0, valb = -8;
    for (uchar c : in)
    {
        if (T[c] == -1)
            break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0)
        {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

std::vector<std::string> tokenize(const std::string &str)
{
    std::vector<std::string> tokens;
    std::string temp = "";
    char delim = ' ';
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == delim)
        {
            tokens.push_back(temp);
            temp = "";
        }
        else
            temp += str[i];
    }
    tokens.push_back(temp);
    return tokens;
}

#endif