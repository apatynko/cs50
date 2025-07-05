#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // ------------------------------------------------------------
    // 1. Validate command-line usage: exactly one argument (the key)
    // ------------------------------------------------------------
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // ------------------------------------------------------------
    // 2. Ensure the key is composed solely of decimal digits
    // ------------------------------------------------------------
    for (int i = 0, len = strlen(argv[1]); i < len; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // ------------------------------------------------------------
    // 3. Convert key from string to int and reduce modulo 26
    // ------------------------------------------------------------
    int key = atoi(argv[1]) % 26;

    // ------------------------------------------------------------
    // 4. Prompt user for plaintext
    // ------------------------------------------------------------
    string plaintext = get_string("plaintext:  ");

    // ------------------------------------------------------------
    // 5. Encrypt using Caesar’s algorithm, preserving case
    // ------------------------------------------------------------
    printf("ciphertext: ");

    for (int i = 0, len = strlen(plaintext); i < len; i++)
    {
        char c = plaintext[i];

        if (isupper(c))
        {
            // Rotate within 'A'..'Z'
            printf("%c", ((c - 'A' + key) % 26) + 'A');
        }
        else if (islower(c))
        {
            // Rotate within 'a'..'z'
            printf("%c", ((c - 'a' + key) % 26) + 'a');
        }
        else
        {
            // Non-alphabetic characters stay the same
            printf("%c", c);
        }
    }

    printf("\n");
    return 0;
}
