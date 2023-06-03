import base64
import functools
import gzip
import json
import os
import random
import time
import tiktoken
from fast_bpe_tokenizer import Tokenizer


def benchmark_tiktoken(documents) -> None:
    num_threads = 1
    num_bytes = sum(map(len, map(str.encode, documents)))
    print(f"num_threads: {num_threads}, num_bytes: {num_bytes}")

    enc = tiktoken.get_encoding("cl100k_base")
    enc.encode("warmup")

    start = time.perf_counter_ns()
    enc.encode_ordinary_batch(documents, num_threads=num_threads)
    end = time.perf_counter_ns()
    print(f"tiktoken \t{num_bytes / (end - start)} GB/s")

def benchmark_fast_bpe_tokenization(documents) -> None:
    total_bytes = 0
    total_time = 0
    found_mismatch = False
    tokenizer = Tokenizer("data/cl100k_base.tiktoken")
    for line in documents:
        total_bytes += len(line.encode('utf-8'))
        start = time.perf_counter_ns()
        token_ids = tokenizer.encode(line)
        end = time.perf_counter_ns()
        total_time += end - start
    print(f"fast_bpe_tokenization \t{total_bytes / (total_time)} GB/s")

lines = open('data/warandpeace.txt').read().splitlines()
benchmark_tiktoken(lines)
benchmark_fast_bpe_tokenization(lines)