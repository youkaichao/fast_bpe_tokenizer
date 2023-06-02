#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "base64.h"

// the size of the character vocabulary
// for example, 256 if the character vocabulary is the set of all 8-bit unsigned integers (i.e. all bytes)
const int CHARACTER_VOCABULARY_SIZE = 256;


// write a class to read a file with suffix ".tiktoken".
// The file represents a vocabulary, which contains many lines, with each line containing a token (encoded by base64) and a token id (int represented as string).
// The vocabulary should be stored into two arrays, one for tokens and the other for token ids.
class Tokenizer {
private:
    // tokens[i] is the index of the first child of node i, the sub-tree lies in [tokens[i], tokens[i] + CHARACTER_VOCABULARY_SIZE)
    // token_ids[i] is the token id of node i, -1 means no such token.
    std::vector<int> tokens;
    std::vector<int> token_ids;

    // a function to peal off the longest token from a certain position, return a pair of (token_id, length)
    std::pair<int, int> peal_off(const std::string& text, int start) const{
        int root = 0;
        int root_token_id = -1;
        int last_found = start -1;
        int last_found_id = -1;
        for (int i = start; i < text.size(); i++) {
            unsigned char x = text[i];
            if (tokens[root + x] == -1) {
                if (root_token_id == -1)
                {
                    return std::make_pair(last_found_id, last_found - start + 1);
                }
                else{
                    last_found = i;
                    last_found_id = root_token_id;
                }
            }
            root_token_id = token_ids[root + x];
            if(root_token_id != -1)
            {
                last_found = i;
                last_found_id = root_token_id;
            }
            root = tokens[root + x];
        }
        if (root_token_id != -1)
        {
            return std::make_pair(root_token_id, text.size() - start);
        }else{
            std::cout << "Error: token not found" << std::endl;
            // out-of-vocabulary error, so we can safely return this pair
            return std::make_pair(last_found_id, last_found - start + 1);
        }        
    }

public:
    Tokenizer(const std::string& filename) {
        // reserve enough space for these two vectors
        std::ifstream file(filename);
        int rough_count = 0;
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            std::string token_id;
            iss >> token >> token_id;
            std::string decoded_bytes = base64_decode(token);
            rough_count += decoded_bytes.size();
        }
        file.close();

        // roughly 164820480 bytes each for cl100k_base.tiktoken
        // std::cout << rough_count * CHARACTER_VOCABULARY_SIZE << std::endl;
        tokens.resize(rough_count * CHARACTER_VOCABULARY_SIZE, -1);
        token_ids.resize(rough_count * CHARACTER_VOCABULARY_SIZE, -1);
        
        // read the file again, and store the tokens and token ids into the vectors
        int start = 0;

        std::ifstream file2(filename);
        while (std::getline(file2, line)) {
            std::istringstream iss(line);
            std::string token;
            std::string token_id;
            iss >> token >> token_id;
            int token_id_int = std::stoi(token_id);
            std::string decoded_bytes = base64_decode(token);
            int root = 0;
            for (int i = 0; i < decoded_bytes.size(); i++) {
                unsigned char x = decoded_bytes[i];

                // record the token id at the parent of leaf node
                if (i == decoded_bytes.size() - 1) {
                    token_ids[root + x] = token_id_int;
                }

                // trace down a tree node.
                // insert a subtree when needed.
                if (tokens[root + x] == -1) {
                    start += CHARACTER_VOCABULARY_SIZE;
                    tokens[root + x] = start;
                    root = start;
                }else{
                    root = tokens[root + x];
                }
            }
        }
        file2.close();

        // shrink tokens and tokens_id to start + CHARACTER_VOCABULARY_SIZE
        // roughly 55488000 bytes each for cl100k_base.tiktoken
        // std::cout << start + CHARACTER_VOCABULARY_SIZE << std::endl;
        tokens.resize(start + CHARACTER_VOCABULARY_SIZE);
        token_ids.resize(start + CHARACTER_VOCABULARY_SIZE);
    }

    // encode a string into a vector of token ids
    std::vector<int> encode(std::string text) const
    {
        std::vector<int> result;

        if(text.size() == 0)
        {
            return result;
        }

        int root = 0;
        int root_token_id = -1;
        int last_found_position = -1;
        int last_found_token_id = -1;

        int i = 0;
        while (i <= text.size())
        {
            unsigned char x;
            if(i == text.size())
            {
                x = 0;
            }else{
                x = text[i];
            }
            if (tokens[root + x] == -1 || i == text.size()) {
                if (root_token_id == -1)
                {
                    // fall back to last_found_position
                    assert (last_found_position != -1);
                    i = last_found_position + 1;
                    result.push_back(last_found_token_id);

                    root = 0;
                    root_token_id = -1;
                    last_found_position = -1;
                    last_found_token_id = -1;
                }
                else{
                    result.push_back(root_token_id);
                    i++;
                    root = 0;
                    root_token_id = -1;
                    last_found_position = i;
                    last_found_token_id = root_token_id;
                }
            }else{
                root_token_id = token_ids[root + x];
                root = tokens[root + x];
                if(root_token_id != -1)
                {
                    last_found_position = i;
                    last_found_token_id = root_token_id;
                }
                i++;
            }
        }
        return std::move(result);
    }
};