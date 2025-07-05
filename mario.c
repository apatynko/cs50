#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;

    // 1. Get a correct number from 1 to 8
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // 2. Draw right-oriented piramid
    for (int i = 0; i < height; i++)
    {
        // print spaces from the left
        for (int j = 0; j < height - 1 - i; j++)
        {
            printf(" ");
        }

        // draw hashes
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        // go to the next row
        printf("\n");
    }
}
