#define _XOPEN_SOURCE   // for crypt
#include <unistd.h>     // crypt
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 5
#define ALPHA   52       // 26 upper- + 26 lower-case

// -------- prototype ---------------------------------------------------------
void brute(char *hash, const char *salt, int depth, char *buffer);

// -----------------------------------------------------------------------------
// MAIN
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./crack hash\n");
        return 1;
    }

    char *hash = argv[1];

    // DES-based crypt salt == first two chars of the hash
    char salt[3];
    strncpy(salt, hash, 2);
    salt[2] = '\0';

    char guess[MAX_LEN + 1] = {0};  // space for '\0'

    brute(hash, salt, 0, guess);    // never returns until password found
    // (If the loop finishes without finding a match — theoretically impossible
    //  given the problem’s guarantees — print nothing and return error.)
    return 2;
}

// -----------------------------------------------------------------------------
// Recursive brute-forcer
// depth  : current position we’re filling [0..MAX_LEN-1]
// buffer : char array where we build the candidate; NUL-terminated on the fly
// -----------------------------------------------------------------------------
void brute(char *hash, const char *salt, int depth, char *buffer)
{
    // When depth > 0 we have a candidate string of length = depth
    if (depth > 0)
    {
        buffer[depth] = '\0';
        if (strcmp(crypt(buffer, salt), hash) == 0)
        {
            printf("%s\n", buffer);
            exit(0);                // success → terminate entire program
        }
    }

    if (depth == MAX_LEN)           // reached maximum length, stop recursion
        return;

    // Generate next character (A–Z then a–z)
    for (int i = 0; i < ALPHA; i++)
    {
        buffer[depth] = (i < 26) ? 'A' + i : 'a' + (i - 26);
        brute(hash, salt, depth + 1, buffer);
    }
}
