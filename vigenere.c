#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// ---------------------------------------------------------------------------
// Return the alphabetic index of c:  A/a → 0,  B/b → 1, …, Z/z → 25
// ---------------------------------------------------------------------------
int shift(char c)
{
    return isupper(c) ? c - 'A' : c - 'a';
}

int main(int argc, string argv[])
{
    // -----------------------------------------------------------------------
    // 1. Validate command-line usage: exactly one alphabetic keyword
    // -----------------------------------------------------------------------
    if (argc != 2)
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }

    string keyword = argv[1];

    for (int i = 0, len = strlen(keyword); i < len; i++)
    {
        if (!isalpha(keyword[i]))
        {
            printf("Usage: ./vigenere keyword\n");
            return 1;
        }
    }

    // -----------------------------------------------------------------------
    // 2. Prompt user for plaintext
    // -----------------------------------------------------------------------
    string plaintext = get_string("plaintext:  ");

    // -----------------------------------------------------------------------
    // 3. Encrypt using the Vigenère cipher
    // -----------------------------------------------------------------------
    printf("ciphertext: ");

    int key_len = strlen(keyword);
    int key_index = 0;                       // which letter of keyword to use

    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        char p = plaintext[i];

        if (isalpha(p))
        {
            int k = shift(keyword[key_index % key_len]);  // current key letter

            if (isupper(p))
            {
                // Rotate within 'A'..'Z'
                printf("%c", ((p - 'A' + k) % 26) + 'A');
            }
            else
            {
                // Rotate within 'a'..'z'
                printf("%c", ((p - 'a' + k) % 26) + 'a');
            }

            key_index++;                    // advance keyword position **only** for letters
        }
        else
        {
            // Non-alphabetic characters are output unchanged
            printf("%c", p);
        }
    }

    printf("\n");
    return 0;
}
