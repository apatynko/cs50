from math import floor

def get_change() -> float:
    """Prompt until the user enters a non-negative amount of dollars."""
    while True:
        try:
            dollars = float(input("Change owed: "))
            if dollars >= 0:
                return dollars
        except ValueError:
            # Ignore non-numeric input and re-prompt
            pass

def compute_min_coins(dollars: float) -> int:
    """
    Return the minimum number of U.S. coins (25¢, 10¢, 5¢, 1¢)
    needed to make the given amount.
    """
    cents = int(round(dollars * 100))           # avoid FP imprecision
    coins = 0
    for value in (25, 10, 5, 1):                # greedy: largest → smallest
        coins += cents // value
        cents  %= value
    return coins

def main() -> None:
    dollars = get_change()
    print(compute_min_coins(dollars))

if __name__ == "__main__":
    main()
