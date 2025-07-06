#!/usr/bin/env python3
import sys

# ------------------------------------------------------------
# 1. Validate command-line usage: exactly one numeric argument
# ------------------------------------------------------------
if len(sys.argv) != 2 or not sys.argv[1].isdigit():
    print("Usage: python caesar.py key")
    sys.exit(1)

# ------------------------------------------------------------
# 2. Convert key to int and reduce modulo 26
# ------------------------------------------------------------
key = int(sys.argv[1]) % 26

# ------------------------------------------------------------
# 3. Prompt user for plaintext
# ------------------------------------------------------------
plaintext = input("plaintext:  ")

# ------------------------------------------------------------
# 4. Encrypt using Caesar’s algorithm, preserving case
# ------------------------------------------------------------
ciphertext_chars = []

for c in plaintext:
    if c.isupper():
        ciphertext_chars.append(chr(((ord(c) - ord('A') + key) % 26) + ord('A')))
    elif c.islower():
        ciphertext_chars.append(chr(((ord(c) - ord('a') + key) % 26) + ord('a')))
    else:                         # non-alphabetic characters unchanged
        ciphertext_chars.append(c)

ciphertext = "".join(ciphertext_chars)
print(f"ciphertext: {ciphertext}")
