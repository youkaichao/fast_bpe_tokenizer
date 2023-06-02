import tiktoken
enc = tiktoken.get_encoding("cl100k_base")
with open('output.txt') as f:
    lines = f.read().splitlines()
    start = 0
    while start < len(lines):
        line = lines[start]
        n = int(lines[start+1])
        token_ids = [int(lines[start+2+i]) for i in range(n)]
        decoded = enc.decode(token_ids)
        if line != decoded:
            print("mismatch")
            print(line)
            print(decoded)
        start += 2 + n