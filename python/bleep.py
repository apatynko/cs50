#!/usr/bin/env python3
"""
bleep.py — цензор повідомлень

Використання:
    python bleep.py banned.txt
"""

import sys
from typing import Set   # для сумісності з Python 3.7

USAGE = "Usage: python bleep.py dictionary"


# ----------------------------------------------------------------------
# Читання словника
# ----------------------------------------------------------------------
def load_banned_words(path: str) -> Set[str]:
    """Повертає множину заборонених слів (у нижньому регістрі)."""
    banned = set()
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            banned.add(line.strip().lower())
    return banned


# ----------------------------------------------------------------------
# Цензура повідомлення
# ----------------------------------------------------------------------
def censor(message: str, banned: Set[str]) -> str:
    """Замінює кожне заборонене слово на зірочки, зберігаючи решту тексту."""
    censored = []
    for token in message.split():
        censored.append("*" * len(token) if token.lower() in banned else token)
    return " ".join(censored)


# ----------------------------------------------------------------------
# Головна логіка
# ----------------------------------------------------------------------
def main() -> None:
    # перевірка аргументів
    if len(sys.argv) != 2:
        print(USAGE)
        sys.exit(1)

    try:
        banned_words = load_banned_words(sys.argv[1])
    except FileNotFoundError:
        print(f"Could not open {sys.argv[1]}")
        sys.exit(1)

    # запит повідомлення
    message = input("What message would you like to censor?\n")

    # вивід результату
    print(censor(message, banned_words))


if __name__ == "__main__":
    main()
