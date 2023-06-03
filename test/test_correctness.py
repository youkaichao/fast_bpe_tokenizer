import time
from fast_bpe_tokenizer import Tokenizer
import tiktoken

enc = tiktoken.get_encoding("cl100k_base")
tokenizer = Tokenizer("data/cl100k_base.tiktoken")

with open('data/warandpeace.txt') as f:
    lines = f.read().splitlines()
    found_mismatch = False
    for line in f:
        token_ids = tokenizer.encode(line)
        decoded = enc.decode(token_ids)
        if line != decoded:
            print("mismatch")
            found_mismatch = True
            print(line)
            print(decoded)
    if not found_mismatch:
        print("perfect decoding!")
