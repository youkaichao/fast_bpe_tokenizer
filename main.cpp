#include "fast_bpe.h"

int main(int argc, char** argv) {
    Tokenizer tokenizer("cl100k_base.tiktoken");

    // read input from a file, and dump the output to a file (one int per line)
    std::ifstream file(argv[1]);
    // open the output file for writing, clear original content
    std::ofstream output_file("output.txt");

    // add timer to timing the tokenization process
    auto start = std::chrono::high_resolution_clock::now();
    long long total_bytes = 0;
    long long total_time = 0;

    std::string line;
    while (std::getline(file, line)) {
        total_bytes += line.size();
        auto start_time = std::chrono::high_resolution_clock::now();
        std::vector<int> result = tokenizer.encode(line);
        auto end_time = std::chrono::high_resolution_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        output_file << line << std::endl;
        output_file << result.size() << std::endl;
        for (int i = 0; i < result.size(); i++) {
            output_file << result[i] << std::endl;
        }
    }
    file.close();
    output_file.close();

    // print the speed of tokenization using total_time and toal_bytes
    std::cout << "Total time: " << total_time << " ns" << std::endl;
    std::cout << "Total bytes: " << total_bytes << " bytes" << std::endl;
    // total_bytes / 1e9 / (total_time / 1e9) = GB/s
    std::cout << "Speed: " << total_bytes * 1.0 / total_time << " GB/s" << std::endl;
    return 0;
}