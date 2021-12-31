#ifndef AES_UTILITIES_H
#define AES_UTILITIES_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <algorithm>

/* Function: print_hex
 * -------------------
 * Takes 16 bytes and prints as hex code.
 *
 * msg: bytes to be printed
 */
void print_hex(char *msg)
{
  for (int i = 0; i < 16; i++)
  {
    printf("%02X ", (unsigned char)*(msg + i));
  }
  putchar('\n');
}

void print_as_hex_table(const std::string &cipher)
{
  int counter = 0;
  int col_cnt = 0;
  for (size_t i = 0; i < cipher.size(); ++i)
  {
    std::cout << cipher[i];
    counter++;
    if (counter == 2)
    {
      counter = 0;
      std::cout << " ";
    }

    if (i >= 31 && (i + 1) % 32 == 0)
    {
      std::cout << " [" << col_cnt++ << "]\n";
    }
  }
  std::cout << "\n";
}

std::string uchar2hex(unsigned char inchar)
{
  std::ostringstream oss(std::ostringstream::out);
  oss << std::setw(2) << std::setfill('0') << std::hex << (int)(inchar);
  return oss.str();
}

std::string convert_ASCII(std::string hex)
{
  std::string ascii = "";
  for (size_t i = 0; i < hex.length(); i += 2)
  {
    //taking two characters from hex std::string
    std::string part = hex.substr(i, 2);
    //changing it into base 16
    char ch = stoul(part, nullptr, 16);
    //putting it into the ASCII std::string
    ascii += ch;
  }
  return ascii;
}

std::string generateSymmetricKey(size_t length)
{
  std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

  std::random_device rd;
  std::mt19937 generator(rd());

  std::shuffle(str.begin(), str.end(), generator);

  return str.substr(0, length);
}

#endif
