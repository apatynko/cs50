#!/usr/bin/env python3
"""
vigenere.py — реалізація шифру Віженера (CS50 Pset6, Sentimental).

Використання:
    python vigenere.py KEY
де KEY — непорожнє слово, що складається лише з літер A-Z / a-z.
"""

import sys
import string

ALPHABET_SIZE = 26
USAGE_MSG = "Usage: python vigenere.py k"

# ----------------------------------------------------------------------
# 1. Перевірка аргументів командного рядка
# ----------------------------------------------------------------------
if len(sys.argv) != 2 or not sys.argv[1].isalpha():
    print(USAGE_MSG)
    sys.exit(1)

key = sys.argv[1]
key_shifts = [ord(c.upper()) - ord('A') for c in key]  # A/a → 0, B/b → 1, …

# ----------------------------------------------------------------------
# 2. Запитуємо plaintext у користувача
# ----------------------------------------------------------------------
plaintext = input("plaintext:  ")

# ----------------------------------------------------------------------
# 3. Шифруємо
# ----------------------------------------------------------------------
ciphertext_chars = []
j = 0  # лічильник, який рухається лише по літерах plaintext

for ch in plaintext:
    if ch.isalpha():
        shift = key_shifts[j % len(key_shifts)]
        base = ord('A') if ch.isupper() else ord('a')
        cipher_ch = chr((ord(ch) - base + shift) % ALPHABET_SIZE + base)
        ciphertext_chars.append(cipher_ch)
        j += 1                      # переходимо до наступної букви ключа
    else:
        ciphertext_chars.append(ch)  # неалфавітні символи без змін

ciphertext = "".join(ciphertext_chars)

# ----------------------------------------------------------------------
# 4. Вивід результату
# ----------------------------------------------------------------------
print(f"ciphertext: {ciphertext}")
