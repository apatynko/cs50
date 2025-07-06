def get_height() -> int:
    """Prompt until the user enters an int from 1 to 8, inclusive."""
    while True:
        try:
            h = int(input("Height: "))
            if 1 <= h <= 8:
                return h
        except ValueError:
            # Ignore non-numeric input and re-prompt
            pass

def draw_double_pyramid(height: int) -> None:
    """Print two side-by-side pyramids of the given height."""
    for row in range(height):
        left_padding = " " * (height - 1 - row)   # 1) leading spaces
        bricks      = "#" * (row + 1)             # 2) row’s hashes
        gap         = "  "                        # 3) fixed two-space gap
        print(left_padding + bricks + gap + bricks)

def main() -> None:
    height = get_height()
    draw_double_pyramid(height)

if __name__ == "__main__":
    main()
