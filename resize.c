/****************************************************************************
 * resize.c
 *
 * Enlarge a 24-bit uncompressed BMP by an integer factor n (1 ≤ n ≤ 100).
 *
 * Usage:  ./resize n infile outfile
 *
 * The program:
 *   1. validates command-line arguments;
 *   2. reads the BMP headers, ensuring format correctness;
 *   3. updates width, height, padding, and size fields;
 *   4. writes the new headers;
 *   5. duplicates each pixel n× horizontally, and each scanline n× vertically;
 *   6. frees any allocated memory and exits with status 0 on success.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "bmp.h"               // CS50 header describing BMP structures

// ---------------------------------------------------------------------------
static void usage(void)
{
    printf("Usage: ./resize n infile outfile\n");
}

// ---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // ---------------------------------------------------- 1. parse arguments
    if (argc != 4)
    {
        usage();
        return 1;
    }

    // factor n
    char *endptr;
    long n_long = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || n_long < 1 || n_long > 100)
    {
        usage();
        return 1;
    }
    int n = (int) n_long;

    char *infile  = argv[2];
    char *outfile = argv[3];

    // ---------------------------------------------------- 2. open files
    FILE *inptr = fopen(infile, "rb");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    FILE *outptr = fopen(outfile, "wb");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 1;
    }

    // ---------------------------------------------------- 3. read headers
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;

    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // verify 24-bit uncompressed BMP 4.0
    bool valid =
        bf.bfType == 0x4d42 &&          // "BM"
        bf.bfOffBits == 54 &&
        bi.biSize == 40 &&
        bi.biBitCount == 24 &&
        bi.biCompression == 0;

    if (!valid)
    {
        fclose(inptr);
        fclose(outptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 1;
    }

    // ---------------------------------------------------- 4. prepare new headers
    int32_t oldWidth  = bi.biWidth;
    int32_t oldHeight = bi.biHeight;     // may be negative (top-down)

    int oldPadding = (4 - (oldWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biWidth  *= n;
    bi.biHeight *= n;

    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biSizeImage = (sizeof(RGBTRIPLE) * bi.biWidth + newPadding) * abs(bi.biHeight);
    bf.bfSize      = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // ---------------------------------------------------- 5. write new headers
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // ---------------------------------------------------- 6. process pixels
    // allocate one scanline of the original image
    RGBTRIPLE *row = malloc(oldWidth * sizeof(RGBTRIPLE));
    if (row == NULL)
    {
        fclose(inptr);
        fclose(outptr);
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0, absH = abs(oldHeight); i < absH; i++)
    {
        // read one original scanline into buffer
        fread(row, sizeof(RGBTRIPLE), oldWidth, inptr);
        fseek(inptr, oldPadding, SEEK_CUR);

        // write that scanline n times (vertical scaling)
        for (int v = 0; v < n; v++)
        {
            // write each pixel n times (horizontal scaling)
            for (int j = 0; j < oldWidth; j++)
            {
                for (int h = 0; h < n; h++)
                {
                    fwrite(&row[j], sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // pad the new scanline
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }
        }
    }

    // ---------------------------------------------------- 7. tidy up
    free(row);
    fclose(inptr);
    fclose(outptr);
    return 0;
}
