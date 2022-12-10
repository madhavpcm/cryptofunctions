#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

void phi_1_to_n(int n, std::vector<int> &phi) {
  phi = std::vector<int>(n + 1);
  for (int i = 0; i <= n; i++)
    phi[i] = i;

  for (int i = 2; i <= n; i++) {
    if (phi[i] == i) {
      for (int j = i; j <= n; j += i)
        phi[j] -= phi[j] / i;
    }
  }
}
/*
 * binary_exponentiation a^b mod n
 **/
int64_t binary_exponentiation(int64_t a, int64_t b, int64_t n) {
  a %= n;
  int64_t res = 1;
  while (b > 0) {
    // check if b is odd or even using bitwise operators for faster calculation
    if (b & 1) {
      // multiply by square of a
      res = (res * a) % n;
    }
    // square a
    a = (a * a) % n;
    // half it using bitwise
    b >>= 1;
  }
  return res;
}
void calculate_inverse_26(std::unordered_map<int, int> &mp,
                          std::unordered_set<int> &all) {
  mp.clear();
  int phi26 = 12;
  mp[1] = 1;

  for (int i = 1; i < 26; i++) {
    if (i % 2 != 0 && i % 13 != 0) {
      mp[i] = binary_exponentiation(i, phi26 - 1, 26);
      all.insert(mp[i]);
    }
  }
}
int main(int argc, char **argv) {
  // affine cipher
  /*
   * k1,k2.
   * P = ( C - k1)*k2 ^ -1 mod 26

      where k1 = {0..25}, k2 = {set of all zero and positive integers < 26 and
  integer is relatively prime with 26}
  **/
  if (argc != 2) {
    std::cout << "[Usage] : ./<exec_name> <input_file>";
    exit(EXIT_FAILURE);
  }

  std::string filename(argv[1]);
  std::string buffer;
  std::string input_string = "";
  std::ifstream input_file(filename);
  std::ofstream output_file("brute_affine.txt");

  while (getline(input_file, buffer)) {
    input_string = input_string + buffer;
  }

  std::unordered_map<int, int> inverse_map;
  std::unordered_set<int> all;
  calculate_inverse_26(inverse_map, all);

  int iter = 0;
  for (int k1 = 0; k1 < 26; k1++) {
    for (auto &k2_ : all) {
      // i, it.second
      // std::cout << *it + i;
      std::string output_string = "";
      for (char &c : input_string) {
        if (c >= 'A' && c <= 'Z') {
          int decry = ((((c - 'A') - k1) * k2_) + 2600) % 26;
          char ch = decry + 'A';
          if (ch < 'A' || ch > 'Z') {
            std::cout << "redflag";
          }
          output_string = output_string + ch;
        } else if (c >= 'a' && c <= 'z') {
          output_string =
              output_string + (char)(((((c - 'a') - k1) * k2_) % 26) + 'a');
        } else {
          output_string = output_string + c;
        }
      }
      output_file << std::string('(' + std::to_string(k1) + ':' +
                                 std::to_string(k2_) + ')' + '\n');
      output_file << output_string;
      output_file << "\n";
      iter = iter + 1;
      // std::cout << output_string << '\n';
    }
  }

  input_file.close();
  output_file.close();
  return 0;
}