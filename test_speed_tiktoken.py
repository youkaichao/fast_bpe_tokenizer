import base64
import functools
import gzip
import json
import os
import random
import time
import tiktoken


def benchmark_batch(documents) -> None:
    num_threads = 1
    num_bytes = sum(map(len, map(str.encode, documents)))
    print(f"num_threads: {num_threads}, num_bytes: {num_bytes}")

    enc = tiktoken.get_encoding("cl100k_base")
    enc.encode("warmup")

    start = time.perf_counter_ns()
    enc.encode_ordinary_batch(documents, num_threads=num_threads)
    end = time.perf_counter_ns()
    print(f"tiktoken \t{num_bytes / (end - start)} GB/s")

lines = open('data/warandpeace.txt').read().splitlines()
benchmark_batch(lines)