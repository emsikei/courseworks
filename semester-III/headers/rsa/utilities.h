#ifndef RSA_UTILITIES_H
#define RSA_UTILITIES_H

#include <iostream>
#include <vector>
#include "utilities.h"

unsigned long long mod(unsigned long long a, unsigned long long b, unsigned long long c)
{
    if (b == 0)
    {
        return 1;
    }
    else if (b % 2 == 0)
    {
        unsigned long long d = mod(a, b / 2, c);
        return (d * d) % c;
    }
    else
    {
        return ((a % c) * mod(a, b - 1, c)) % c;
    }
}

void getMessage(std::string &message, std::vector<unsigned long long> &M)
{
    char *arr = &message[0];

    std::string two_numbers = "";
    for (int i = 0; i < message.length(); ++i)
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

void convertToNumbers(unsigned long long &D, unsigned long long &N, std::vector<std::string> &M)
{
    std::vector<unsigned long long> C;
    for (int i = 0; i < M.size(); ++i)
    {
        C.push_back(std::stoll(M[i]));
    }

    M.clear();
    for (int i = 0; i < C.size(); ++i)
    {
        M.push_back(std::to_string(mod(C[i], D, N)));
    }
}

std::string formMessage(std::vector<std::string> &M)
{
    std::string Mssg;
    for (int i = 0; i < M.size(); ++i)
    {
        if (M[i].length() == 4)
        {
            std::string number = "";
            number += M[i][0];
            number += M[i][1];
            Mssg += (char)(std::stod(number) + 31);
            number = "";
            number += M[i][2];
            number += M[i][3];
            Mssg += (char)(std::stod(number) + 31);
        }
        else
        {
            std::string number = "";
            number += M[i][0];
            Mssg += (char)(std::stod(number) + 31);
            number = "";
            number += M[i][1];
            number += M[i][2];
            Mssg += (char)(std::stod(number) + 31);
        }
    }

    return Mssg;
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

int calculate_E(unsigned long long Phi_N)
{
    int i = 2;
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

int calculate_D(unsigned long long Phi_N, unsigned long long E)
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

#endif