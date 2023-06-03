from fast_bpe_tokenizer._Tokenizer import Tokenizer as cppTokenizer
import os

class Tokenizer:
    def __init__(self, vocab_file: str):
        assert os.path.exists(vocab_file), f"Vocab file {vocab_file} does not exist"
        self.tokenizer = cppTokenizer(vocab_file)

    def encode(self, text: str):
        return self.tokenizer.encode(text.encode('utf-8'))
