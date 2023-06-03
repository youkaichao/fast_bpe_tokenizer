# fast_bpe_tokenizer
fast bpe tokenizer, simple to understand, easy to use

# Usage

Install: `pip install -e . -vvv`

Usage:

```python
from fast_bpe_tokenizer import Tokenizer
tokenizer = Tokenizer("data/cl100k_base.tiktoken")
print(tokenizer.encode('Hello, world!'))
```

A testcase is to tokenize the whole book of "War and Peace": `python test/test_correctness.py` .

There is also a testcase to compare the speed vs the speed of `tiktoken`: `python test/test_speed.py`.

On my laptop, it prints:

```text
num_threads: 1, num_bytes: 3158163
tiktoken        0.002757442995720329 GB/s
fast_bpe_tokenization   0.0321009567095709 GB/s
```

That is to say, `fast_bpe_tokenization` achieves 32.0 MB/s encoding speed on my laptop. In contrast, `tiktoken` only achieves 2.8 MB/s.

On my Linux server, the speed test prints:

```text
num_threads: 1, num_bytes: 3158163
tiktoken 	0.0013368014069854459 GB/s
fast_bpe_tokenization 	0.027368681306682224 GB/s
```

Spuriously, on my Linux server, `tiktoken` is slower than my laptop.

This implementation is at least 10x faster than `tiktoken`!

# Resolving Ambiguity

With a given vocabulary, there might be many different ways to encode the same string. For example, if the vocabulary contains "a"/"b"/"ab", then the string "ab" can be tokenized as a single token "ab", or it can be tokenized as two tokens "a" and "b".

There are two possible ways to resolve such ambiguity:
- Maximum Prefix Encoding: Choose the first token with max-length ("ab" in this case). This leads to fewer tokens, and faster model inference.
- Maximum Priority Encoding: Choose the first token with max-priority (i.e., the least token id. in this case, the token with max priority is "a"). This favors frequent tokens, and might lead to better textual understanding, at the cost of more tokens and slower model inference.

This implementation follows a strict maximum prefix encoding. Maximum Priority Encoding can also be easily implemented.

However, I don't know which rule `tiktoken` follows. The following examples show that `tiktoken` sometimes follows Maximum Priority Encoding (when encoding ` BPE` and `tiktoken`), but other times follows Maximum Prefix Encoding (when encoding `aaaaa`).

```python
import tiktoken
enc = tiktoken.get_encoding("cl100k_base")
for i in [30167, 36, 426, 1777]:
    token = enc.decode([i])
    print(f'token {token}, id {i}')
text = ' BPE'
print(f'The tokenization of {text} is {enc.encode(text)}')
for i in [10462, 5964, 1713, 83, 1609, 5963]:
    token = enc.decode([i])
    print(f'token {token}, id {i}')
text = 'tiktoken'
print(f'The tokenization of {text} is {enc.encode(text)}')
for i in [64, 29558]:
    token = enc.decode([i])
    print(f'token {token}, id {i}')
text = 'aaaaa'
print(f'The tokenization of {text} is {enc.encode(text)}')
```

The output is (notice the leading space in token 30167 and 426):
```text
token  BP, id 30167
token E, id 36
token  B, id 426
token PE, id 1777
The tokenization of  BPE is [426, 1777]
token ti, id 10462
token kt, id 5964
token oken, id 1713
token t, id 83
token ik, id 1609
token token, id 5963
The tokenization of tiktoken is [83, 1609, 5963]
token a, id 64
token aaaa, id 29558
The tokenization of aaaaa is [29558, 64]
```

# Understanding the details of tiktoken

This implementation is written when I try to understand the details of `tiktoken`. The tutorial can be found in the `tiktoken_explained.ipynb`. It is interesting to find that my implementation turns out to be much faster than the `tiktoken` implementation.

# Acknowledgement

The base64 decoding in C++ comes from https://github.com/ReneNyffenegger/cpp-base64 .

This repository follows MIT License. Feel free to use and modify the code.

# Future work

- [ ] Add support for special tokens
- [x] Add python wrappers
- [ ] Add multi-threading support
