#include <Eigen/Dense>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

class DES {
private:
  std::vector<std::vector<int>> pdbox{{{57, 49, 41, 33, 25, 17, 9, 1},
                                       {58, 50, 42, 34, 26, 18, 10, 2},
                                       {59, 51, 43, 35, 27, 19, 11, 3},
                                       {60, 52, 44, 36, 63, 55, 47, 39},
                                       {31, 23, 15, 7, 62, 54, 46, 38},
                                       {30, 22, 14, 6, 61, 53, 45, 37},
                                       {29, 21, 13, 5, 28, 20, 12, 4}}};

  std::vector<std::vector<int>> kcbox{{{14, 17, 11, 24, 1, 5, 3, 28},
                                       {15, 6, 21, 10, 23, 19, 12, 4},
                                       {26, 8, 16, 7, 27, 20, 13, 2},
                                       {41, 52, 31, 37, 47, 55, 30, 40},
                                       {51, 45, 33, 48, 44, 49, 39, 56},
                                       {34, 53, 46, 42, 50, 36, 29, 32}}};

  std::vector<std::vector<int>> stbox{{{16, 7, 20, 21, 29, 12, 28, 17},
                                       {1, 15, 23, 26, 5, 18, 31, 10},
                                       {2, 8, 24, 14, 32, 27, 3, 9},
                                       {19, 13, 30, 6, 22, 11, 4, 25}}};

  std::vector<std::vector<int>> expbox{{{32, 1, 2, 3, 4, 5},
                                        {4, 5, 6, 7, 8, 9},
                                        {8, 9, 10, 11, 12, 13},
                                        {12, 13, 14, 15, 16, 17},
                                        {16, 17, 18, 19, 20, 21},
                                        {20, 21, 22, 23, 24, 25},
                                        {24, 25, 26, 27, 28, 29},
                                        {28, 29, 30, 31, 32, 1}}};

  std::vector<std::vector<int>> ibox{{{58, 50, 42, 34, 26, 18, 10, 2},
                                      {60, 52, 44, 36, 28, 20, 12, 4},
                                      {62, 54, 46, 38, 30, 22, 14, 6},
                                      {64, 56, 48, 40, 32, 24, 16, 8},
                                      {57, 49, 41, 33, 25, 17, 9, 1},
                                      {59, 51, 43, 35, 27, 19, 11, 3},
                                      {61, 53, 45, 37, 29, 21, 13, 5},
                                      {63, 55, 47, 39, 31, 23, 15, 7}}};

  std::vector<std::vector<int>> fbox{{{40, 8, 48, 16, 56, 24, 64, 32},
                                      {39, 7, 47, 15, 55, 23, 63, 31},
                                      {38, 6, 46, 14, 54, 22, 62, 30},
                                      {37, 5, 45, 13, 53, 21, 61, 29},
                                      {36, 4, 44, 12, 52, 20, 60, 28},
                                      {35, 3, 43, 11, 51, 19, 59, 27},
                                      {34, 2, 42, 10, 50, 18, 58, 26},
                                      {33, 1, 41, 9, 49, 17, 57, 25}}};

  std::vector<std::vector<std::vector<int>>> sbox{
      {{{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
       {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
       {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
       {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
       {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
       {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
       {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
       {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}}};

public:
  DES(uint64_t key);
  std::vector<uint64_t> round_keys;
  uint64_t initial_permutation(uint64_t &input);
  uint64_t final_permutation(uint64_t &input);

  uint64_t encrypt(uint64_t &plainText);
  uint64_t decrypt(uint64_t &plainText);

  void round_mixer(uint32_t &leftBlock, uint32_t &rightBlock,
                   uint64_t &roundKey);
  uint64_t round_function(uint32_t &input, uint64_t &roundKey);
  uint64_t expand32_48(uint32_t &input);
  uint32_t substitute(uint64_t &input);
  uint32_t straight32_32(uint32_t &input);

  void keygen(uint64_t key);
};
DES::DES(uint64_t key) {
  round_keys = std::vector<uint64_t>(16, 0);
  keygen(key);
}

// definitions

/**
 *
 * Des encryption algorithm, from Behrouz  Forouzan
 *
 * Cipher() function
 */
uint64_t DES::encrypt(uint64_t &plainText) {
  uint64_t inBlock = initial_permutation(plainText);

  uint32_t lBlock = (inBlock >> 32) & 0x00000000ffffffff;
  uint32_t rBlock = inBlock & 0x00000000ffffffff;

  for (int round = 0; round < 16; round++) {
    std::cout << "0x" << std::setfill('0') << std::setw(8) << std::right
              << std::hex << lBlock << ' ';
    std::cout << "0x" << std::setfill('0') << std::setw(8) << std::right
              << std::hex << rBlock << std::endl;
    round_mixer(lBlock, rBlock, round_keys[round]);
    if (round != 15) {
      std::swap(lBlock, rBlock);
    }
  }

  uint64_t outBlock = 0;
  outBlock |= (lBlock);
  outBlock <<= 32;
  outBlock |= (uint64_t)(rBlock);

  outBlock = final_permutation(outBlock);
  return outBlock;
}
/*
 * DES decryption
 */
uint64_t DES::decrypt(uint64_t &plainText) {
  uint64_t inBlock = initial_permutation(plainText);

  uint32_t lBlock = (inBlock >> 32) & 0x00000000ffffffff;
  uint32_t rBlock = inBlock & 0x00000000ffffffff;

  for (int round = 0; round < 16; round++) {
    round_mixer(lBlock, rBlock, round_keys[15 - round]);
    if (round != 15) {
      std::swap(lBlock, rBlock);
    }
  }

  uint64_t outBlock = 0;
  outBlock |= (lBlock);
  outBlock <<= 32;
  outBlock |= (uint64_t)(rBlock);

  outBlock = final_permutation(outBlock);
  return outBlock;
}

/*
 * Mixer used in each fiestel cipher step
 */
void DES::round_mixer(uint32_t &leftBlock, uint32_t &rightBlock,
                      uint64_t &roundKey) {
  uint32_t t1 = rightBlock;
  uint32_t t2 = round_function(t1, roundKey);
  uint32_t t3 = t2 ^ leftBlock;
  leftBlock = t3;
}

/*
 * f(i,ki), part of the fiestel cipher in DES
 */
uint64_t DES::round_function(uint32_t &input, uint64_t &roundKey) {
  uint64_t t1 = expand32_48(input);

  uint64_t t2 = roundKey ^ t1;

  uint32_t t3 = substitute(t2);

  return straight32_32(t3);
}

/*
 * 32 bit to 48 bit expansion p box
 */

uint64_t DES::expand32_48(uint32_t &input) {
  uint64_t output = 0;

  for (int i = 0; i < 48; i++) {
    output <<= 1;
    output |= (uint64_t)(((input >> (32 - expbox[i / 6][i % 6])) & 0x1));
    // output[i] = input[expbox[i / 6][i % 6] - 1];
  }
  return output;
}
/*
 * SBox 48 to 32 in DES
 */
uint32_t DES::substitute(uint64_t &input) {
  uint32_t output = 0;
  for (int i = 0; i < 8; i++) {
    // 84 -> 1000 0100
    ///     *******
    unsigned char row =
        (unsigned char)((input & (0x0000840000000000 >> 6 * i)) >>
                        (42 - 6 * i));

    row = (row >> 4) | (row & 0x1);
    // 78-> 0111 1000
    ///     *******
    unsigned char col =
        (unsigned char)((input & (0x0000780000000000 >> 6 * i)) >>
                        (43 - 6 * i));

    output <<= 4;
    output |= ((uint32_t)(sbox[i][row][col]) & 0xf);
  }
  return output;
}
/*
 * 32 bit to 32 bit straight p box
 */
uint32_t DES::straight32_32(uint32_t &input) {
  uint32_t output = 0;

  for (int i = 0; i < 32; i++) {
    output <<= 1;
    output |= ((uint32_t)((input >> (32 - stbox[i / 8][i % 8])) & 0x1));
    // output[i] = input[expbox[i / 6][i % 6] - 1];
  }
  return output;
}

/*
 * Initial P Box
 */
uint64_t DES::initial_permutation(uint64_t &input) {
  uint64_t output = 0;
  for (int i = 0; i < 64; i++) {
    output <<= 1;
    output |= (input >> (64 - ibox[i / 8][i % 8])) & 1ULL;
  }
  return output;
}
/*
 * Final P box, or inversion of initial P box
 */
uint64_t DES::final_permutation(uint64_t &input) {
  uint64_t output = 0;
  for (int i = 0; i < 64; i++) {
    output <<= 1;
    output |= (input >> (64 - fbox[i / 8][i % 8])) & 1ULL;
  }
  return output;
}

/*
 * DES Keygen
 */
void DES::keygen(uint64_t key) {
  uint64_t key56 = 0;
  for (int i = 0; i < 56; i++) {
    key56 <<= 1;
    // get the transformed bit, and set it
    key56 |= (key >> (64 - pdbox[i / 8][i % 8])) & 1ULL;
  }

  uint32_t leftKey28 = 0;
  uint32_t rightKey28 = 0;

  leftKey28 = (key56 >> 28) & 0x000000000fffffff;
  rightKey28 = key56 & 0x000000000fffffff;

  for (int round = 0; round < 16; round++) {
    if (round == 0 || round == 1 || round == 8 || round == 15) {
      uint32_t b;
      b = (leftKey28 >> 27) & 1;
      leftKey28 <<= 1;
      leftKey28 |= b;
      leftKey28 &= 0x000000000fffffff;
      b = (rightKey28 >> 27) & 1;
      rightKey28 <<= 1;
      rightKey28 |= b;
      rightKey28 &= 0x000000000fffffff;

    } else {
      uint32_t b;
      b = (leftKey28 >> 26) & 3;
      leftKey28 <<= 2;
      leftKey28 |= b;
      leftKey28 &= 0x000000000fffffff;
      b = (rightKey28 >> 26) & 3;
      rightKey28 <<= 2;
      rightKey28 |= b;
      rightKey28 &= 0x000000000fffffff;
    }

    // combine the keys
    uint64_t preRoundKey56 = 0;
    preRoundKey56 |= ((uint64_t)leftKey28);
    preRoundKey56 <<= 28;
    preRoundKey56 |= ((uint64_t)rightKey28);

    uint64_t roundKey = 0;
    for (int i = 0; i < 48; i++) {
      roundKey <<= 1;
      // get the transformed bit, and set it
      roundKey |= (preRoundKey56 >> (56 - kcbox[i / 8][i % 8])) & 1ULL;
    }
    round_keys[round] = roundKey;
  }
}
/*
 * Main program, accepts a file with data to be encrypted
 */
int main(int argc, char **argv) {
  //   uint64_t key = 0xaabb09182736ccdd;
  uint64_t key = 0xcfbb99162238c019;

  //   uint64_t p = 0x123456abcd132536;

  //   std::string p = "CatLovesFoodYEYEYA";
  //
  std::string p;
  std::cin >> p;

  std::vector<uint64_t> pstream;
  std::vector<uint64_t> cstream;

  // String to Hex
  for (int i = 0; i < p.size(); i += 8) {
    std::string strchunk = p.substr(i, 8);

    if (strchunk.size() != 8) {
      // padding
      while (strchunk.size() != 8)
        strchunk += '*';
    }

    uint64_t chunk64 = 0;
    for (int i = 0; i < 8; i++) {
      uint8_t k = static_cast<uint8_t>(strchunk[i]);
      chunk64 |= (uint64_t)(k);
      if (i != 7)
        chunk64 <<= 8;
    }
    pstream.emplace_back(chunk64);
  }

  DES instance(key);

  for (auto &p : pstream) {
    cstream.emplace_back(instance.encrypt(p));
  }

  std::vector<uint64_t> pstream_;

  for (auto &c : cstream) {
    pstream_.emplace_back(instance.decrypt(c));
  }

  // Hex to string
  //
  //
  std::string decrypted;
  for (auto &p_ : pstream_) {
    std::string strchunk;
    for (int i = 0; i < 8; i++) {
      uint8_t uc = (uint8_t)((p_ >> (56 - i * 8)) & 0xff);

      char c = static_cast<char>(uc);
      strchunk += c;
    }
    decrypted += strchunk;
  }

  std::cout << decrypted << std::endl;
  //   uint64_t c = instance.encrypt(p);

  //   std::cout << std::hex << c << std::endl;

  //   uint64_t p_ = instance.decrypt(c);

  //   std::cout << std::hex << p_ << std::endl;

  return 0;
}