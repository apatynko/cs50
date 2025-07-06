def get_card_number() -> int:
    """Prompt until the user types a positive integer."""
    while True:
        num_str = input("Number: ").strip()
        if num_str.isdigit():          # all characters 0-9
            return int(num_str)

def luhn_valid(number: int) -> bool:
    """Return True if `number` passes the Luhn checksum."""
    total = 0
    for pos, digit_char in enumerate(reversed(str(number))):
        digit = int(digit_char)

        if pos % 2 == 1:               # every second digit from the right
            doubled = digit * 2
            total += doubled // 10 + doubled % 10   # add the digits of the product
        else:
            total += digit

    return total % 10 == 0

def issuer(number: int) -> str:
    """Identify the card issuer or return 'INVALID'."""
    num_str   = str(number)
    length    = len(num_str)
    first_one = int(num_str[0])
    first_two = int(num_str[:2])

    if luhn_valid(number):
        if length == 15 and first_two in (34, 37):
            return "AMEX"
        if length == 16 and 51 <= first_two <= 55:
            return "MASTERCARD"
        if length in (13, 16) and first_one == 4:
            return "VISA"

    return "INVALID"

def main() -> None:
    number = get_card_number()
    print(issuer(number))

if __name__ == "__main__":
    main()
