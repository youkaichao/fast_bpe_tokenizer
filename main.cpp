#include "fast_bpe.h"

int main() {
    Tokenizer tokenizer("cl100k_base.tiktoken");

    // read input from a file, and dump the output to a file (one int per line)
    std::ifstream file("input.txt");
    std::ofstream output_file("output.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> result = tokenizer.encode(line);
        output_file << line << std::endl;
        for (int i = 0; i < result.size(); i++) {
            output_file << result[i] << std::endl;
        }
    }
    return 0;
}