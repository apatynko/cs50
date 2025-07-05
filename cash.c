#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float dollars;

    // 1. Prompt user for a non-negative amount, re-prompt until it is valid
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);

    // 2. Convert dollars to cents, rounding to avoid floating-point imprecision
    int cents = (int) round(dollars * 100);

    // 3. Greedy algorithm: count coins from largest to smallest
    int coins = 0;

    // 25-cent coins
    coins += cents / 25;
    cents  = cents % 25;

    // 10-cent coins
    coins += cents / 10;
    cents  = cents % 10;

    // 5-cent coins
    coins += cents / 5;
    cents  = cents % 5;

    // 1-cent coins
    coins += cents;      // whatever is left are pennies

    // 4. Output the minimal number of coins followed by a newline
    printf("%i\n", coins);
}
