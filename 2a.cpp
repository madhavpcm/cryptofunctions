#include <Eigen/Dense>
#include <fstream>
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
  std::vector<int> initial_permutation(std::vector<int> &input);
  std::vector<int> final_permutation(std::vector<int> &input);
  std::vector<int> expand32_48(std::vector<int> &input);
  std::vector<int> straight32_32(std::vector<int> &input);
  std::vector<int> XoR(std::vector<int> &a, std::vector<int> &b);
  std::vector<int> encrypt(std::vector<int> &plainText,
                           std::vector<std::vector<int>> &roundKeys);
  void round_mixer(std::vector<int> &leftBlock, std::vector<int> &rightBlock,
                   std::vector<int> &roundKey);
  std::vector<int> round_function(std::vector<int> &input,
                                  std::vector<int> &roundKey);
  std::vector<int> substitute(std::vector<int> &input);
  std::vector<std::vector<int>> keygen();
  uint64_t DES::drop_parity_bits(uint64_t key);
};
// definitions

/**
 *
 * Des encryption algorithm, from Behrouz  Forouzan
 *
 * Cipher() function
 */
std::vector<int> DES::encrypt(std::vector<int> &plainText,
                              std::vector<std::vector<int>> &roundKeys) {

  std::vector<int> inblock = std::move(initial_permutation(plainText));

  std::vector<int> rightBlock =
      std::move(std::vector<int>(inblock.begin() + 32, inblock.begin() + 64));
  std::vector<int> leftBlock =
      std::move(std::vector<int>(inblock.begin(), inblock.begin() + 32));

  for (int round = 0; round < 16; round++) {
    round_mixer(leftBlock, rightBlock, roundKeys[round]);
    if (round != 16) {
      std::swap(leftBlock, rightBlock);
    }
  }
  std::vector<int> output;
  output.reserve(64);
  // memory efficient copy
  output.insert(output.end(), std::make_move_iterator(leftBlock.begin()),
                std::make_move_iterator(leftBlock.end()));
  output.insert(output.end(), std::make_move_iterator(rightBlock.begin()),
                std::make_move_iterator(rightBlock.end()));
}
/*
 * SBox 48 to 32 in DES
 */
std::vector<int> DES::substitute(std::vector<int> &input) {
  std::vector<int> output(32, 0);
  for (int i = 0; i < 8; i++) {
    int row = 2 * input[i * 6 + 1] + input[i * 6 + 6];
    int col = 8 * input[i * 6 + 2] + 4 * input[i * 6 + 3] +
              2 * input[i * 6 + 4] + input[i * 6 + 5];

    int val = sbox[i][row][col];
    output[i * 4 + 1] = val / 8;
    output[i * 4 + 2] = val / 4;
    output[i * 4 + 3] = val / 2;
    output[i * 4 + 4] = val;
  }
  return std::move(output);
}
/*
 * bitwise XoR
 */
std::vector<int> DES::XoR(std::vector<int> &a, std::vector<int> &b) {
  if (a.size() != b.size()) {
    std::cout << "XoR error";
  }

  std::vector<int> c(a.size(), 0);
  for (int i = 0; i < a.size(); i++) {
    c[i] = a[i] ^ b[i];
  }
  return std::move(c);
}
/*
 * 32 bit to 48 bit expansion p box
 */

std::vector<int> DES::expand32_48(std::vector<int> &input) {
  std::vector<int> output(48, 0);

  for (int i = 0; i < 48; i++) {
    output[i] = input[expbox[i / 6][i % 6] - 1];
  }
  return std::move(output);
}
/*
 * 32 bit to 32 bit straight p box
 */
std::vector<int> DES::straight32_32(std::vector<int> &input) {
  std::vector<int> output(32, 0);

  if (input.size() != 32) {
    std::cout << "straight pbox needs a 32 bit input \n";
  }

  for (int i = 0; i < 32; i++) {
    output[i] = input[stbox[i / 8][i % 8] - 1];
  }
  return std::move(output);
}
/*
 * f(i,ki), part of the fiestel cipher in DES
 */
std::vector<int> DES::round_function(std::vector<int> &input,
                                     std::vector<int> &roundKey) {
  std::vector<int> t1(std::move(expand32_48(input)));

  std::vector<int> t2(std::move(XoR(t1, roundKey)));

  std::vector<int> t3(std::move(substitute(t2)));

  return std::move(straight32_32(t3));
}

/*
 * Mixer used in each fiestel cipher step
 */
void DES::round_mixer(std::vector<int> &leftBlock, std::vector<int> &rightBlock,
                      std::vector<int> &roundKey) {
  std::vector<int> t1(rightBlock.begin(), rightBlock.end());
  std::vector<int> t2(std::move(round_function(t1, roundKey)));
  std::vector<int> t3(std::move(XoR(leftBlock, t2)));
  rightBlock = std::move(t3);
}

/*
 * Initial P Box
 */
std::vector<int> DES::initial_permutation(std::vector<int> &input) {
  if (input.size() != 8 * 8) {
    std::cout << "Init box needs 64 bit input";
  }

  std::vector<int> output(64, 0);
  for (int i = 0; i < 64; i++) {
    output[fbox[i / 8][i % 8] - 1] = input[i];
  }
  return std::move(output);
}
uint64_t DES::drop_parity_bits(uint64_t key) {}
/*
 * Final P box, or inversion of initial P box
 */
std::vector<int> DES::final_permutation(std::vector<int> &input) {
  if (input.size() != 8 * 8) {
    std::cout << "Final box needs 64 bit input";
  }

  std::vector<int> output;
  for (int i = 0; i < 64; i++) {
    output[ibox[i / 8][i % 8] - 1] = input[i];
  }
  return std::move(output);
}
/*
 * DES Keygen
 */
std::vector<std::vector<int>> keygen(uint64_t key) {}
/*
 * Main program, accepts a file with data to be encrypted
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    exit(EXIT_FAILURE);
  }

  std::string filename(argv[1]);
  std::string buffer;
  std::string input_string = "";
  std::ifstream input_file(filename);
  std::ofstream output_file("vignere_decrypt.txt");

  std::string key = "AABB09182736CCDD";

  while (getline(input_file, buffer)) {
    input_string += buffer;
  }
  if (!input_string.size()) {
    std::cout << "File is empty \n";
    exit(EXIT_FAILURE);
  }

  return 0;
}