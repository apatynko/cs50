/***************************************************************************
 * recover.c
 *
 * Recovers all JPEGs from a forensic “card” image.
 *
 * Usage: ./recover image
 *        where image is a raw memory-card dump (e.g. card.raw)
 *
 * The program scans the card 512 bytes at a time, detects JPEG
 * signatures, and writes each found JPEG to ###.jpg (000, 001, …).
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK 512                 // size of one FAT block

int main(int argc, char *argv[])
{
    /* ------------------------------------------------------- 1. arguments */
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    /* ------------------------------------------------------- 2. open input */
    FILE *card = fopen(argv[1], "rb");
    if (card == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    /* ------------------------------------------------------- 3. buffers    */
    uint8_t buffer[BLOCK];         // holds one 512-byte block
    FILE   *img = NULL;            // current JPEG we’re writing to
    int     jpg_count = 0;         // how many JPEGs we’ve found
    char    filename[8];           // "###.jpg" + '\0'

    /* ------------------------------------------------------- 4. scan card  */
    while (fread(buffer, sizeof(uint8_t), BLOCK, card) == BLOCK)
    {
        bool is_start =
            buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0;   // 0xe0, 0xe1, … 0xef

        if (is_start)
        {
            /* if already writing a JPEG, close it */
            if (img != NULL)
            {
                fclose(img);
            }

            /* create new JPEG file */
            sprintf(filename, "%03i.jpg", jpg_count++);
            img = fopen(filename, "wb");
            if (img == NULL)
            {
                fclose(card);
                fprintf(stderr, "Could not create %s\n", filename);
                return 3;
            }
        }

        /* if inside a JPEG, write the current block */
        if (img != NULL)
        {
            fwrite(buffer, sizeof(uint8_t), BLOCK, img);
        }
    }

    /* ------------------------------------------------------- 5. cleanup    */
    if (img != NULL)
        fclose(img);

    fclose(card);
    return 0;
}
