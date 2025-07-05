#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;

    // Ask for a height between 1 and 8 (inclusive)
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Draw each row
    for (int i = 0; i < height; i++)
    {
        // 1. Left padding: height-1-i spaces
        for (int j = 0; j < height - 1 - i; j++)
        {
            printf(" ");
        }

        // 2. Left pyramid: i+1 hashes
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        // 3. Gap between pyramids: exactly two spaces
        printf("  ");

        // 4. Right pyramid: i+1 hashes
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        // 5. Move to the next line
        printf("\n");
    }
}
