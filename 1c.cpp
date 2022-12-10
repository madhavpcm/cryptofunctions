#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// std::vector<std::vector<int>> matmul(std::vector<std::vector<int>> &a,
//                                      std::vector<std::vector<int>> &b) {
//   int m1, n1, m2, n2;
//   m1 = a.size();
//   n1 = a[0].size();
//   m2 = b.size();
//   n2 = b[0].size();

//   if (n1 != m2) {
//     return {};
//   }

//   std::vector<std::vector<int>> c(m1, std::vector<int>(n2, 0));

//   for (int i = 0; i < m1; i++) {
//     for (int k = 0; k < n2; k++) {
//       int sumij = 0;
//       for (int j = 0; j < n1; j++) {
//         sumij += a[i][j] * b[j][k];
//       }
//       c[i][k] = sumij;
//     }
//   }
//   return std::move(c);
// }

// std::vector<std::vector<int>> matinv(std::vector<std::vector<int>> &mat) {
//   int m = mat.size();
//   int n = mat[0].size();

//   if (m != n) {
//     return {};
//   }
// }
#define toNormal(x) ((x) - 'A')
#define toAscii(x) ((x) + 'A')
#define C 2
#define O 14
#define N 13
#define E 4
#define T 19
#define D 3
#define G 6
#define R 17
#define A 0
#define P 15
#define H 7
#define I 8

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "usage: ./<exec_name> <input_file>" << std::endl;
    return -1;
  }

  std::string filename(argv[1]);
  std::string buffer;
  std::string input_string = "";
  std::string output_string = "";
  std::ifstream input_file(filename);
  std::ofstream output_file("hill.txt");

  while (getline(input_file, buffer)) {
    input_string += buffer;
  }
  Eigen::MatrixXd key(4, 4);
  key << C, O, N, N, E, C, T, E, D, G, R, A, P, H, I, C;

  Eigen::MatrixXd key_(4, 4);
  key_ = key.inverse();
  std::cout << key_ << '\n';

  for (int i = 0; i < input_string.size(); i += 16) {
    Eigen::MatrixXd bi(4, 1);
    std::string ci = input_string.substr(i, 4);
    bi << toNormal(ci[0]), toNormal(ci[1]), toNormal(ci[2]), toNormal(ci[3]);
    // toNormal(ci[4]), toNormal(ci[5]), toNormal(ci[6]), toNormal(ci[7]),
    // toNormal(ci[8]), toNormal(ci[9]), toNormal(ci[10]), toNormal(ci[11]),
    // toNormal(ci[12]), toNormal(ci[13]), toNormal(ci[14]), toNormal(ci[15]);

    std::cout << bi << '\n';
    Eigen::MatrixXd bi_(4, 1);

    bi_.noalias() = key_ * bi;

    std::cout << bi_ << '\n';
    std::string pi;
    for (auto &c : bi_.reshaped()) {
      c *= 26;
      c = fmod(c, 26);
    }

    std::cout << bi_ << '\n';
    for (auto &c : bi_.reshaped()) {
      pi += toAscii(c);
    }
    output_string += pi;
  }

  std::cout << key << '\n' << key_;

  return 0;
}