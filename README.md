# fast_bpe_tokenizer
fast bpe tokenizer, simple to understand, easy to use

# Resolving Ambiguity

With a given vocabulary, there might be many different ways to encode the same string. For example, if the vocabulary contains "a"/"b"/"ab", then the string "ab" can be tokenized as a single token "ab", or it can be tokenized as two tokens "a" and "b".

There are two possible ways to resolve such ambiguity:
- Maximum Prefix Encoding: Choose the first token with max-length ("ab" in this case).
- Maximum Priority Encoding: Choose the first token with max-priority (i.e., the least token id. in this case, the token with max priority is "a").

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

