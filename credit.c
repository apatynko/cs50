#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // ---------- 1. Get input ----------
    long long number = get_long("Number: ");

    // ---------- 2. Luhn checksum ----------
    int sum = 0;
    int position = 0;          // 0 = last digit, 1 = second-to-last, …
    long long n = number;      // copy so we can destroy it

    while (n > 0)
    {
        int digit = n % 10;    // current right-most digit

        if (position % 2 == 1) // every other digit, starting with second-to-last
        {
            int doubled = digit * 2;
            sum += doubled / 10 + doubled % 10; // add the two digits of the product
        }
        else
        {
            sum += digit;
        }

        n /= 10;
        position++;
    }

    bool valid = (sum % 10 == 0);

    // ---------- 3. Determine length and leading digits ----------
    int length = position;

    // first two digits
    long long leading = number;
    while (leading >= 100)            // reduce until only two digits remain
    {
        leading /= 10;
    }
    int first_two = (int) leading;
    int first_one = first_two / 10;

    // ---------- 4. Identify issuer ----------
    if (valid)
    {
        if (length == 15 && (first_two == 34 || first_two == 37))
        {
            printf("AMEX\n");
            return 0;
        }
        if (length == 16 && (first_two >= 51 && first_two <= 55))
        {
            printf("MASTERCARD\n");
            return 0;
        }
        if ((length == 13 || length == 16) && first_one == 4)
        {
            printf("VISA\n");
            return 0;
        }
    }

    // Anything that reaches here is invalid
    printf("INVALID\n");
}
