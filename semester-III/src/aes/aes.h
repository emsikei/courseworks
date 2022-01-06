#ifndef AES_H
#define AES_H

#include <iostream>
#include <cstring>
#include "lookup_boxes.h"
#include "utilities.h"
#include "constants.h"

/* ================================= Signatures ================================= */

std::string AES_Encrypt(const std::string &message, const std::string &key);
std::string AES_Decrypt(const std::string &encryptedMessage, const std::string &key);
char *aes_encrypt(char *message, unsigned char *expanded_key);
char *aes_decrypt(std::string message, unsigned char *expanded_key);

void keyExpansion(unsigned char *input_key, unsigned char *expanded_key);
void addRoundKey(unsigned char *state, unsigned char *round_key);
void subBytes(unsigned char *state);
void shiftRows(unsigned char *state);
void mixColumns(unsigned char *state);
void invSubBytes(unsigned char *state);
void invShiftRows(unsigned char *state);
void invMixColumns(unsigned char *state);

void keyExpansionCore(unsigned char *in, unsigned char i);
char *right_pad_str(const char *str, unsigned int pad_len);

/* ================================= Implementation ================================= */

std::string AES_Encrypt(const std::string &message, const std::string &key)
{
  const char *str = message.c_str();

  char *padded_msg = right_pad_str(str, 16);
  unsigned int padded_msg_len = strlen(str);
  if (padded_msg_len % 16)
    padded_msg_len = (padded_msg_len / 16 + 1) * 16;

  unsigned char expanded_key[176];

  unsigned char *input_key = new unsigned char[key.length() + 1];
  std::copy(key.begin(), key.end(), input_key);
  input_key[key.length()] = 0;

  keyExpansion(input_key, expanded_key);

  char *enc_msg;
  std::string encryptedMessage;

  for (unsigned int i = 0; i < padded_msg_len; i += 16)
  {
    enc_msg = aes_encrypt(padded_msg + i, expanded_key);

    for (int i = 0; i < 16; i++)
    {
      encryptedMessage += uchar2hex((unsigned char)*(enc_msg + i));
    }

    delete[] enc_msg;
  }

  delete[] padded_msg;

  return encryptedMessage;
}

std::string AES_Decrypt(const std::string &encryptedMessage, const std::string &key)
{
  std::string msg = convert_ASCII(encryptedMessage);

  unsigned char expanded_key[176];

  unsigned char *input_key = new unsigned char[key.length() + 1];
  std::copy(key.begin(), key.end(), input_key);
  input_key[key.length()] = 0;

  keyExpansion(input_key, expanded_key);

  char *dec_msg;

  std::string decryptedMessage = "";

  for (unsigned int i = 0; i < msg.length(); i += 16)
  {
    std::string tmp = msg;
    tmp.erase(0, i);
    dec_msg = aes_decrypt(tmp, expanded_key);
    decryptedMessage += reinterpret_cast<char *>(dec_msg);;
    delete[] dec_msg;
  }
  std::cout << std::endl;

  return decryptedMessage;
}

char *aes_encrypt(char *message, unsigned char *expanded_key)
{
  const unsigned int ROUND_CNT = 9;

  unsigned char state[16];

  for (int i = 0; i < 16; i++)
    state[i] = message[i];

  addRoundKey(state, expanded_key);

  for (unsigned int i = 0; i < ROUND_CNT; i++)
  {
    subBytes(state);
    shiftRows(state);
    mixColumns(state);
    addRoundKey(state, expanded_key + (16 * (i + 1)));
  }

  subBytes(state);
  shiftRows(state);
  addRoundKey(state, expanded_key + 160);

  char *enc_msg = new char[16];
  memcpy(enc_msg, state, 16);
  return enc_msg;
}

char *aes_decrypt(std::string message, unsigned char *expanded_key)
{
  const unsigned int ROUND_CNT = 9;

  unsigned char state[16];

  for (int i = 0; i < 16; i++)
    state[i] = message[i];

  addRoundKey(state, expanded_key + 160);

  for (int i = ROUND_CNT; i > 0; i--)
  {
    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, expanded_key + (16 * i));
    invMixColumns(state);
  }
  invShiftRows(state);
  invSubBytes(state);
  addRoundKey(state, expanded_key);

  char *dec_msg = new char[16];
  memcpy(dec_msg, state, 16);
  return dec_msg;
}

void keyExpansionCore(unsigned char *in, unsigned char i)
{
  // Циклический сдвиг четырех в байтов влево на 1 и
  // замена каждого байта в соотсвествии с
  // таблицей подстановок s_box
  in[0] = s_box[in[0]];
  in[1] = s_box[in[1]];
  in[2] = s_box[in[2]];
  in[3] = s_box[in[3]];

  // RCon XOR Rcon[i] = (RC[i], 0X00, 0X00, 0X00)
  in[0] ^= rcon[i];
}

void keyExpansion(unsigned char *input_key, unsigned char *expanded_key)
{
  // Копирование первых 16 байтов ключа в expanded_key
  for (int i = 0; i < 16; i++)
    expanded_key[i] = input_key[i];

  // Первые 16 байтов уже были скопированы
  unsigned int bytes_generated = 16;
  int rcon_iteration = 1;
  unsigned char temp[4];

  while (bytes_generated < 176)
  {
    // Считывание первых четырех байтов
    for (int i = 0; i < 4; i++)
      temp[i] = expanded_key[i + bytes_generated - 4];

    // Выполнение keyExpansionCore один раз для каждого 16-байтного ключа
    if (bytes_generated % 16 == 0)
      keyExpansionCore(temp, rcon_iteration++);

    // XOR temp с [bytes_generated-16], и сохранение в expanded_key
    for (unsigned char a = 0; a < 4; a++)
    {
      expanded_key[bytes_generated] = expanded_key[bytes_generated - 16] ^ temp[a];
      bytes_generated++;
    }
  }
}

void subBytes(unsigned char *state)
{
  // Substitute each state value with another byte in the Rijndael S-Box
  for (int i = 0; i < 16; i++)
    state[i] = s_box[state[i]];
}

void invSubBytes(unsigned char *state)
{
  for (int i = 0; i < 16; i++)
    state[i] = inv_s_box[state[i]];
}

void shiftRows(unsigned char *state)
{
  unsigned char tmp[16];

  tmp[0] = state[0];
  tmp[4] = state[4];
  tmp[8] = state[8];
  tmp[12] = state[12];

  tmp[1] = state[5];
  tmp[5] = state[9];
  tmp[9] = state[13];
  tmp[13] = state[1];

  tmp[2] = state[10];
  tmp[6] = state[14];
  tmp[10] = state[2];
  tmp[14] = state[6];

  tmp[3] = state[15];
  tmp[7] = state[3];
  tmp[11] = state[7];
  tmp[15] = state[11];

  for (int i = 0; i < 16; i++)
    state[i] = tmp[i];
}

void invShiftRows(unsigned char *state)
{
  unsigned char tmp[16];

  tmp[0] = state[0];
  tmp[4] = state[4];
  tmp[8] = state[8];
  tmp[12] = state[12];

  tmp[1] = state[13];
  tmp[5] = state[1];
  tmp[9] = state[5];
  tmp[13] = state[9];

  tmp[2] = state[10];
  tmp[6] = state[14];
  tmp[10] = state[2];
  tmp[14] = state[6];

  tmp[3] = state[7];
  tmp[7] = state[11];
  tmp[11] = state[15];
  tmp[15] = state[3];

  for (int i = 0; i < 16; i++)
    state[i] = tmp[i];
}

void mixColumns(unsigned char *state)
{
  unsigned char tmp[16];

  tmp[0] = (unsigned char)(mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3]);
  tmp[1] = (unsigned char)(state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3]);
  tmp[2] = (unsigned char)(state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]]);
  tmp[3] = (unsigned char)(mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]]);

  tmp[4] = (unsigned char)(mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7]);
  tmp[5] = (unsigned char)(state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7]);
  tmp[6] = (unsigned char)(state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]]);
  tmp[7] = (unsigned char)(mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]]);

  tmp[8] = (unsigned char)(mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11]);
  tmp[9] = (unsigned char)(state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11]);
  tmp[10] = (unsigned char)(state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]]);
  tmp[11] = (unsigned char)(mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]]);

  tmp[12] = (unsigned char)(mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15]);
  tmp[13] = (unsigned char)(state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15]);
  tmp[14] = (unsigned char)(state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]]);
  tmp[15] = (unsigned char)(mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]]);

  for (int i = 0; i < 16; i++)
    state[i] = tmp[i];
}

void invMixColumns(unsigned char *state)
{
  unsigned char tmp[16];

  tmp[0] = (unsigned char)(mul14[state[0]] ^ mul11[state[1]] ^ mul13[state[2]] ^ mul9[state[3]]);
  tmp[1] = (unsigned char)(mul9[state[0]] ^ mul14[state[1]] ^ mul11[state[2]] ^ mul13[state[3]]);
  tmp[2] = (unsigned char)(mul13[state[0]] ^ mul9[state[1]] ^ mul14[state[2]] ^ mul11[state[3]]);
  tmp[3] = (unsigned char)(mul11[state[0]] ^ mul13[state[1]] ^ mul9[state[2]] ^ mul14[state[3]]);

  tmp[4] = (unsigned char)(mul14[state[4]] ^ mul11[state[5]] ^ mul13[state[6]] ^ mul9[state[7]]);
  tmp[5] = (unsigned char)(mul9[state[4]] ^ mul14[state[5]] ^ mul11[state[6]] ^ mul13[state[7]]);
  tmp[6] = (unsigned char)(mul13[state[4]] ^ mul9[state[5]] ^ mul14[state[6]] ^ mul11[state[7]]);
  tmp[7] = (unsigned char)(mul11[state[4]] ^ mul13[state[5]] ^ mul9[state[6]] ^ mul14[state[7]]);

  tmp[8] = (unsigned char)(mul14[state[8]] ^ mul11[state[9]] ^ mul13[state[10]] ^ mul9[state[11]]);
  tmp[9] = (unsigned char)(mul9[state[8]] ^ mul14[state[9]] ^ mul11[state[10]] ^ mul13[state[11]]);
  tmp[10] = (unsigned char)(mul13[state[8]] ^ mul9[state[9]] ^ mul14[state[10]] ^ mul11[state[11]]);
  tmp[11] = (unsigned char)(mul11[state[8]] ^ mul13[state[9]] ^ mul9[state[10]] ^ mul14[state[11]]);

  tmp[12] = (unsigned char)(mul14[state[12]] ^ mul11[state[13]] ^ mul13[state[14]] ^ mul9[state[15]]);
  tmp[13] = (unsigned char)(mul9[state[12]] ^ mul14[state[13]] ^ mul11[state[14]] ^ mul13[state[15]]);
  tmp[14] = (unsigned char)(mul13[state[12]] ^ mul9[state[13]] ^ mul14[state[14]] ^ mul11[state[15]]);
  tmp[15] = (unsigned char)(mul11[state[12]] ^ mul13[state[13]] ^ mul9[state[14]] ^ mul14[state[15]]);

  for (int i = 0; i < 16; i++)
    state[i] = tmp[i];
}

void addRoundKey(unsigned char *state, unsigned char *round_key)
{
  for (int i = 0; i < 16; i++)
    state[i] ^= round_key[i];
}

char *right_pad_str(const char *str, unsigned int pad_len)
{
  const unsigned int str_len = strlen(str);
  unsigned int padded_str_len = str_len;
  if (padded_str_len % pad_len != 0)
    padded_str_len = (padded_str_len / pad_len + 1) * pad_len;

  char *padded_str = new char[padded_str_len];
  for (unsigned int i = 0; i < padded_str_len; i++)
  {
    if (i >= str_len)
      padded_str[i] = 0;
    else
      padded_str[i] = str[i];
  }
  return padded_str;
}

#endif
