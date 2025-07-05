/****************************************************************************
 * whodunit.c
 *
 * Remove red noise from clue.bmp to reveal the hidden picture.
 *
 * Usage:  ./whodunit infile outfile
 *
 * The program expects a 24-bit uncompressed BMP 4.0 file.  It copies the
 * headers unchanged and then, for every pixel, checks its colour.  Any pixel
 * whose red component dominates (   red ≥ 200  &&  green ≤ 100  &&  blue ≤ 100 )
 * is turned to pure white (255,255,255); everything else is copied verbatim.
 *
 * Result: the red scribbles disappear and the underlying image becomes clear.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define _XOPEN_SOURCE     // (does nothing here, left from copy.c skeleton)

#include "bmp.h"          // CS50 header describing BMP file formats

// Helper: is this pixel “mostly red”?
static inline bool is_mostly_red(RGBTRIPLE px)
{
    return px.rgbtRed   >= 200   &&   // high red
           px.rgbtGreen <= 100   &&   // little green
           px.rgbtBlue  <= 100;       // little blue
}

int main(int argc, char *argv[])
{
    // --------------------------------------------------------------------
    // 0.  Command-line check
    // --------------------------------------------------------------------
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 1;
    }

    char *infile  = argv[1];
    char *outfile = argv[2];

    // --------------------------------------------------------------------
    // 1.  Open input file
    // --------------------------------------------------------------------
    FILE *inptr = fopen(infile, "rb");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    // --------------------------------------------------------------------
    // 2.  Open/create output file
    // --------------------------------------------------------------------
    FILE *outptr = fopen(outfile, "wb");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 1;
    }

    // --------------------------------------------------------------------
    // 3.  Read and copy the BMP headers
    // --------------------------------------------------------------------
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;

    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // quick sanity-check: 24-bit uncompressed BMP 4.0 ?
    if (bf.bfType != 0x4d42 ||            // “BM”
        bf.bfOffBits != 54  ||
        bi.biSize != 40      ||
        bi.biBitCount != 24  ||
        bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 1;
    }

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // --------------------------------------------------------------------
    // 4.  Determine padding per scanline
    // --------------------------------------------------------------------
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // --------------------------------------------------------------------
    // 5.  Iterate over each scanline
    // --------------------------------------------------------------------
    for (int i = 0; i < abs(bi.biHeight); i++)
    {
        // pixel loop
        for (int j = 0; j < bi.biWidth; j++)
        {
            RGBTRIPLE pixel;
            fread(&pixel, sizeof(RGBTRIPLE), 1, inptr);

            // if pixel is mostly red, turn it into white
            if (is_mostly_red(pixel))
            {
                pixel.rgbtRed   = 0xff;
                pixel.rgbtGreen = 0xff;
                pixel.rgbtBlue  = 0xff;
            }

            fwrite(&pixel, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding in the input file
        fseek(inptr, padding, SEEK_CUR);

        // then add identical padding to the output file
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // --------------------------------------------------------------------
    // 6.  Clean up
    // --------------------------------------------------------------------
    fclose(inptr);
    fclose(outptr);
    return 0;   // success
}
