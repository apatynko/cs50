#!/usr/bin/env python3
"""
crack.py — злам пароля, захешованого DES-crypt.

Використання:
    python crack.py HASH
де HASH — 13-символьний хеш (перші дві літери — salt).
"""

import crypt
import itertools
import string
import sys

ALPHABET = string.ascii_letters          # 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
MAX_LEN  = 5                             # паролі до 5 символів

def brute_force(hash_to_crack: str) -> str | None:
    """
    Повертає пароль, що відповідає 'hash_to_crack',
    або None, якщо не знайдено (теоретично не трапляється).
    """
    salt = hash_to_crack[:2]

    for length in range(1, MAX_LEN + 1):
        # Генеруємо всі комбінації потрібної довжини.
        for candidate_tuple in itertools.product(ALPHABET, repeat=length):
            candidate = ''.join(candidate_tuple)
            if crypt.crypt(candidate, salt) == hash_to_crack:
                return candidate
    return None

def main() -> None:
    # --- 1. Перевірка аргументів командного рядка --------------------------
    if len(sys.argv) != 2:
        print("Usage: python crack.py hash")
        sys.exit(1)

    target_hash = sys.argv[1]

    # --- 2. Підбір пароля ---------------------------------------------------
    password = brute_force(target_hash)

    # --- 3. Виведення результату -------------------------------------------
    if password is not None:
        print(password)
    else:
        # На випадок, якщо хеш поза межами передбачених умов
        print("Password not found")

if __name__ == "__main__":
    main()
