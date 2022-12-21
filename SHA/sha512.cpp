#include <algorithm>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class SHA512 {
private:
  std::vector<uint64_t> h0{{0x6a09e667f3bcc908, 0xbb67ae8584caa73b,
                            0x3c6ef372ef94f82b, 0xa54fe53a5f1d36f1,
                            0x510e527fade682d1, 0x9b05688c2b3e6c1f,
                            0x1f83d9abfb41bd6b, 0x5be0cd19137e2179}};

  std::vector<uint64_t> h{{0x6a09e667f3bcc908, 0xbb67ae8584caa73b,
                           0x3c6ef372ef94f82b, 0xa54fe53a5f1d36f1,
                           0x510e527fade682d1, 0x9b05688c2b3e6c1f,
                           0x1f83d9abfb41bd6b, 0x5be0cd19137e2179}};

  std::vector<uint64_t> words;

  std::vector<uint64_t> round_constants{
      0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
      0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
      0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
      0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
      0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
      0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
      0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
      0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
      0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
      0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
      0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
      0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
      0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
      0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
      0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
      0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
      0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
      0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
      0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
      0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
      0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
      0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
      0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
      0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
      0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
      0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
      0x5fcb6fab3ad6faec, 0x6c44198c4a475817};

public:
  SHA512() : words(std::vector<uint64_t>(80, 0)){};
  std::vector<bool> wordgen(std::vector<bool> &message);
  std::vector<uint64_t> chunkify(std::string m);
  std::vector<uint64_t> hash(std::vector<uint64_t> &message,
                             std::vector<uint64_t> hash);
  uint64_t rotShift(size_t l, size_t m, size_t n, uint64_t x);
  std::vector<uint64_t> round(std::vector<uint64_t> input, size_t iter);
  uint64_t mixer1(uint64_t a, uint64_t b, uint64_t c);
  uint64_t majority(uint64_t a, uint64_t b, uint64_t c) {

    return ((a & b) ^ (b & c) ^ (c & a));
  }
  uint64_t conditional(uint64_t a, uint64_t b, uint64_t c) {
    return ((a & b) ^ (~a & c));
  }
  uint64_t rotateA(uint64_t x);
  uint64_t rotateE(uint64_t x);
  uint64_t mixer2(uint64_t d, uint64_t e, uint64_t f, uint64_t h,
                  uint64_t iter);
};
/*
 * SHA512 Rotate in Forouzan
 */
uint64_t SHA512::rotateA(uint64_t a) {
  return ((a >> 28) | (a << (64 - 28))) ^ ((a >> 34) | (a << (64 - 34))) ^
         ((a >> 39) | (a << (64 - 39)));
}
/*
 * SHA512 Rotate in Forouzan
 */
uint64_t SHA512::rotateE(uint64_t a) {
  return ((a >> 14) | (a << (64 - 14))) ^ ((a >> 18) | (a << (64 - 18))) ^
         ((a >> 41) | (a << (64 - 41)));
}
/*
 * SHA512 Mixer 1 in Forouzan
 */
uint64_t SHA512::mixer1(uint64_t a, uint64_t b, uint64_t c) {
  uint64_t t1 = majority(a, b, c);
  uint64_t t2 = rotateA(a);

  return t1 + t2;
}
/*
 * SHA512 Mixer 2 in Forouzan
 */
uint64_t SHA512::mixer2(uint64_t e, uint64_t f, uint64_t g, uint64_t h,
                        uint64_t iter) {
  uint64_t t1 = conditional(e, f, g);
  std::cout << std::setfill('0') << std::hex << std::setw(16) << std::right
            << t1 << '\n';
  uint64_t t2 = rotateA(e);

  return h + (t1 + t2 + round_constants[iter] + words[iter]);
}
/*
 * SHA512 Round, part of the 80 round process
 */
std::vector<uint64_t> SHA512::round(std::vector<uint64_t> input, size_t iter) {
  std::vector<uint64_t> output(8, 0);
  uint64_t k1 = mixer1(input[0], input[1], input[2]);
  uint64_t k2 = mixer2(input[4], input[5], input[6], input[7], iter);

  output[0] = k1 + k2;
  output[1] = input[0];
  output[2] = input[1];
  output[3] = input[2];
  output[4] = k2 + input[3];
  output[5] = input[4];
  output[6] = input[5];
  output[7] = input[6];

  return output;
}
/*
 * SHA512 RotShift used for generating 80 words;
 */
uint64_t SHA512::rotShift(size_t l, size_t m, size_t n, uint64_t x) {
  uint64_t rl = ((x >> l) | (x << (64 - l)));
  uint64_t rm = ((x >> m) | (x << (64 - m)));
  uint64_t sl = (x >> n);

  return rl ^ rm ^ sl;
}
/*
 * SHA512 hash process
 */
std::vector<uint64_t> SHA512::hash(std::vector<uint64_t> &message1024,
                                   std::vector<uint64_t> hash) {
  // Each word is 64 bit
  // 64 * 16 = 1024 bit
  //
  if (hash.size() != 0) {
    h = hash;
  } else {
    h = h0;
  }

  words.clear();
  words.reserve(80);
  // First 16 words
  for (int i = 0; i < 16; i++) {
    words[i] = message1024[i];
  }

  for (int i = 16; i < 80; i++) {
    // Word generation
    uint64_t wi_16 = words[i - 16];
    uint64_t wi_15 = rotShift(1, 8, 7, words[i - 15]);
    uint64_t wi_7 = words[i - 7];
    uint64_t wi_2 = rotShift(19, 61, 6, words[i - 2]);

    words[i] = (wi_16 + wi_15 + wi_7 + wi_2);
  }

  // Input
  std::vector<uint64_t> input(h);
  std::vector<uint64_t> output(h);

  // 80 Rounds
  for (int i = 0; i < 80; i++) {
    // std::cout << "Round : " << i << " Input" << std::endl;
    for (auto &j : input)
      // std::cout << "0x" << std::setfill('0') << std::setw(16) << std::right
      //           << std::hex << j << std::endl;
      input = round(input, i);
  }

  // Final Addition
  for (int i = 0; i < 8; i++) {
    output[i] += input[i];
  }

  return std::move(output);
}
// Convert an input string to 1024 bit chunks
std::vector<uint64_t> SHA512::chunkify(std::string m) {
  size_t original_size = m.size() * 8;

  size_t padding = ((-original_size - 128) % 1024 + 1024) % 1024;

  std::vector<uint64_t> blocks;
  bool flag = true;
  for (int i = 0; i < m.size(); i += 8) {
    uint64_t block = 0;
    // grab 8 chars
    for (int j = i; j < i + 8; j++) {
      // tuck it in
      if (j >= m.size()) {
        // pad 1
        flag = false;
        block |= (uint64_t)(0x80);
        block <<= (7 - j) * 8;
        break;
      }
      uint8_t c = static_cast<uint8_t>(m[j]);
      block |= (uint64_t)c;
      if (j != i + 7) {
        block <<= 8;
      }
    }
    blocks.emplace_back(block);
  }

  size_t considered = ((8 - m.size() % 8) * 8) % 64;

  padding -= considered;

  for (int i = 0; i < padding / 64; i++) {
    if (flag) {
      blocks.emplace_back(uint64_t(1ULL << 63ULL));
      flag = false;
    } else {
      blocks.emplace_back(uint64_t(0));
    }
  }

  std::bitset<128> length(original_size);

  // this may not working if size is above 64 as to_ullong may fail, however for
  // now let it be
  blocks.emplace_back((length >> 64).to_ullong());
  blocks.emplace_back((length.to_ullong()));

  return std::move(blocks);
}
int main(int argc, char **argv) {
  SHA512 instance;

  std::string m = "abc";
  std::vector<uint64_t> chunks(std::move(instance.chunkify(m)));

  for (auto &hashchunk : chunks) {
    // std::cout << "0x" << std::setfill('0') << std::setw(16) << std::right
    //           << std::hex << hashchunk << ' ' << std::endl;
  }
  std::vector<uint64_t> hash = {};
  for (int i = 0; i < chunks.size(); i++) {
    // take 16 chunks each
    std::vector<uint64_t> chunk =
        std::vector<uint64_t>(chunks.begin() + i, chunks.begin() + i + 16);
    // generate hash with 16 chunks
    // hash is 64bit * 8 = 512bit
    if (i == 0) {
      hash = instance.hash(chunk, hash);
    }
  }

  for (auto &hashchunk : hash) {
    std::cout << "0x" << std::setfill('0') << std::setw(16) << std::right
              << std::hex << hashchunk << ' ';
  }

  return 0;
}