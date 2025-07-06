def get_height() -> int:
    """Prompt the user for a height between 1 and 8 (inclusive)."""
    while True:
        try:
            height = int(input("Height: "))
            if 1 <= height <= 8:
                return height
        except ValueError:
            # Ignore non-numeric input and re-prompt
            pass

def draw_pyramid(height: int) -> None:
    """Draw a right-aligned pyramid of the given height."""
    for row in range(height):
        spaces = " " * (height - 1 - row)     # Leading spaces
        hashes = "#" * (row + 1)              # Pyramid blocks
        print(spaces + hashes)

def main() -> None:
    height = get_height()
    draw_pyramid(height)

if __name__ == "__main__":
    main()
