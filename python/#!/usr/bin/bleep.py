#!/usr/bin/env python3
"""
bleep.py — цензор повідомлень

Використання:
    python bleep.py banned.txt
де banned.txt — текстовий файл з одним забороненим словом на рядок.
"""

import sys

USAGE_MSG = "Usage: python bleep.py dictionary"


def load_banned_words(path: str) -> set[str]:
    """
    Повертає множину слів (у нижньому регістрі), прочитаних з файлу `path`.
    У кожному рядку файлу — рівно одне слово.
    """
    banned = set()
    with open(path, "r", encoding="utf-8") as file:
        for line in file:
            banned.add(line.strip().lower())
    return banned


def censor_message(message: str, banned: set[str]) -> str:
    """
    Замінює кожне слово з `banned` на той самий набір зірочок.
    Порівняння виконується без урахування регістру.
    """
    censored_tokens = []
    for token in message.split():
        if token.lower() in banned:
            censored_tokens.append("*" * len(token))
        else:
            censored_tokens.append(token)
    return " ".join(censored_tokens)


def main() -> None:
    # ------------------------------------------------------------
    # 1. Перевірка аргументів командного рядка
    # ------------------------------------------------------------
    if len(sys.argv) != 2:
        print(USAGE_MSG)
        sys.exit(1)

    dictionary_path = sys.argv[1]

    # ------------------------------------------------------------
    # 2. Завантаження списку заборонених слів
    # ------------------------------------------------------------
    try:
        banned_words = load_banned_words(dictionary_path)
    except FileNotFoundError:
        print(f"Could not open {dictionary_path}")
        sys.exit(1)

    # ------------------------------------------------------------
    # 3. Запитуємо повідомлення в користувача
    # ------------------------------------------------------------
    message = input("What message would you like to censor?\n")

    # ------------------------------------------------------------
    # 4. Виводимо зацензуроване повідомлення
    # ------------------------------------------------------------
    print(censor_message(message, banned_words))


if __name__ == "__main__":
    main()
