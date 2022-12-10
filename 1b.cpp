#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define toNormal(x) ((x) - 'A')
#define toAscii(x) ((x) + 'A')

int main(int argc, char **argv) {
  if (argc != 2) {
    exit(EXIT_FAILURE);
  }

  std::string filename(argv[1]);
  std::string buffer;
  std::string input_string = "";
  std::ifstream input_file(filename);
  std::ofstream output_file("vignere_decrypt.txt");

  std::string key = "INTEGRITY";
  while (getline(input_file, buffer)) {
    input_string = input_string + buffer;
  }

  int j = 0;
  for (auto &c : input_string) {
    c = toAscii((toNormal(c) - toNormal(key[j]) + 26) % 26);
    j = (j + 1) % key.size();
  }

  output_file << input_string;

  output_file.close();
  input_file.close();

  return 0;
}