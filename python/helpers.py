"""
helpers.py
Алгоритми пошуку подібностей у двох рядках тексту.
"""

from nltk.tokenize import sent_tokenize
import nltk

# гарантуємо, що токенізатор пунктуації встановлений
try:
    nltk.data.find("tokenizers/punkt")
except LookupError:
    nltk.download("punkt", quiet=True)


def lines(a: str, b: str):
    """
    Повернути усі унікальні РЯДКИ (line-by-line), які одночасно
    зустрічаються і в a, і в b.
    """
    # splitlines() залишає порожні рядки (""), якщо у тексті було "\n\n"
    set_a = set(a.splitlines())
    set_b = set(b.splitlines())

    # множинна перетинка → list, відсортована для сталого порядку
    return sorted(set_a & set_b)


def sentences(a: str, b: str):
    """
    Повернути всі унікальні РЕЧЕННЯ англійською, що є і в a, і в b.
    """
    set_a = set(sent_tokenize(a))
    set_b = set(sent_tokenize(b))

    return sorted(set_a & set_b)


def substrings(a: str, b: str, n: int):
    """
    Повернути всі унікальні підрядки довжиною n,
    які трапляються і в a, і в b.
    """
    if n <= 0:
        return []

    def _ngrams(s: str, k: int):
        """Допоміжна: усі підрядки довжини k у рядку s."""
        return {s[i : i + k] for i in range(len(s) - k + 1)}

    set_a = _ngrams(a, n)
    set_b = _ngrams(b, n)

    return sorted(set_a & set_b)
