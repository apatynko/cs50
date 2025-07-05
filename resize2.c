/****************************************************************************
 * resize.c  —  arbitrary-factor BMP resizer   (CS50 “more” version)
 *
 * Usage:  ./resize f infile outfile
 *
 * where   0.0 < f ≤ 100.0
 *
 * The program:
 *   • validates arguments and BMP format;
 *   • loads the entire input image into memory;
 *   • computes new dimensions = round(old * f);
 *   • rewrites headers (width, height, padding, image and file sizes);
 *   • writes the output image, picking source pixels via nearest neighbour;
 *   • frees all allocated memory; exits 0 on success.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "bmp.h"                 // CS50 header for BITMAPFILEHEADER / INFO

static void usage(void)
{
    printf("Usage: ./resize f infile outfile\n");
}

int main(int argc, char *argv[])
{
    /* -------------------------------------------------- 1. parse arguments */
    if (argc != 4)
    {
        usage();
        return 1;
    }

    char  *endptr;
    double f = strtod(argv[1], &endptr);

    if (*endptr != '\0' || f <= 0.0 || f > 100.0)
    {
        usage();
        return 1;
    }

    char *infile  = argv[2];
    char *outfile = argv[3];

    /* -------------------------------------------------- 2. open files       */
    FILE *inptr = fopen(infile, "rb");
    if (!inptr)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }
    FILE *outptr = fopen(outfile, "wb");
    if (!outptr)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 1;
    }

    /* -------------------------------------------------- 3. read headers     */
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    fread(&bf, sizeof(bf), 1, inptr);
    fread(&bi, sizeof(bi), 1, inptr);

    bool ok =
        bf.bfType == 0x4d42 &&
        bf.bfOffBits == 54 &&
        bi.biSize == 40 &&
        bi.biBitCount == 24 &&
        bi.biCompression == 0;
    if (!ok)
    {
        fclose(inptr);
        fclose(outptr);
        fprintf(stderr, "Unsupported BMP format.\n");
        return 1;
    }

    /* ----------------------------- 4. store original dimensions & padding   */
    int32_t oldW = bi.biWidth;
    int32_t oldH = bi.biHeight;          // may be negative (top-down BMP)
    int     oldPadding = (4 - (oldW * sizeof(RGBTRIPLE)) % 4) % 4;

    /* -------------------------------------------------- 5. load pixels      */
    int absOldH = abs(oldH);
    RGBTRIPLE **img = malloc(absOldH * sizeof(RGBTRIPLE *));
    if (!img)
    {
        fclose(inptr);
        fclose(outptr);
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < absOldH; i++)
    {
        img[i] = malloc(oldW * sizeof(RGBTRIPLE));
        if (!img[i])
        {
            fprintf(stderr, "Memory allocation failed.\n");
            while (i--) free(img[i]);
            free(img);
            fclose(inptr);
            fclose(outptr);
            return 1;
        }
        fread(img[i], sizeof(RGBTRIPLE), oldW, inptr);
        fseek(inptr, oldPadding, SEEK_CUR);
    }

    /* -------------------------------------------------- 6. compute new geometry */
    int32_t newW = (int32_t) lrint(oldW * f);   // round to nearest int
    int32_t newH = (int32_t) lrint(oldH * f);   // keeps sign of oldH

    if (newW == 0 || newH == 0)                 // avoid degenerate result
    {
        fprintf(stderr, "Factor too small — new size rounds to 0.\n");
        for (int i = 0; i < absOldH; i++) free(img[i]);
        free(img);
        fclose(inptr);
        fclose(outptr);
        return 1;
    }

    int newPadding = (4 - (newW * sizeof(RGBTRIPLE)) % 4) % 4;

    /* adjust headers */
    bi.biWidth  = newW;
    bi.biHeight = newH;
    bi.biSizeImage = (sizeof(RGBTRIPLE) * newW + newPadding) * abs(newH);
    bf.bfSize      = bi.biSizeImage + sizeof(bf) + sizeof(bi);

    /* -------------------------------------------------- 7. write new headers */
    fwrite(&bf, sizeof(bf), 1, outptr);
    fwrite(&bi, sizeof(bi), 1, outptr);

    /* -------------------------------------------------- 8. write new pixels  */
    double rowScale = (double) absOldH / abs(newH);
    double colScale = (double) oldW     / newW;

    for (int i = 0; i < abs(newH); i++)
    {
        /* source row (nearest neighbour) */
        int srcRow = (int) floor(i * rowScale);
        if (oldH < 0)                       // top-down BMP: rows stored top→bottom
            srcRow = i * rowScale;
        else                                // bottom-up BMP: rows stored bottom→top
            srcRow = absOldH - 1 - (int) floor(i * rowScale);

        for (int j = 0; j < newW; j++)
        {
            int srcCol = (int) floor(j * colScale);
            RGBTRIPLE px = img[srcRow][srcCol];
            fwrite(&px, sizeof(RGBTRIPLE), 1, outptr);
        }
        /* padding */
        for (int k = 0; k < newPadding; k++)
            fputc(0x00, outptr);
    }

    /* -------------------------------------------------- 9. clean up          */
    for (int i = 0; i < absOldH; i++) free(img[i]);
    free(img);
    fclose(inptr);
    fclose(outptr);
    return 0;
}
