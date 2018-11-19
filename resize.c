// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize N infile outfile\n");
        return 1;
    }

    //obtain N resize factor
    int N = atoi(argv[1]);

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];


    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // determine padding for INfile scanlines
    int paddingIN = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // save INfile height and width in new variable
    int widthIN = bi.biWidth;
    int heightIN = bi.biHeight;

    // change outfile width and height by factor-N
    bi.biWidth *= N;
    bi.biHeight *= N;

    // determine padding for OUTfile scanlines
    int paddingOUT = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine Image size
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + paddingOUT) * abs(bi.biHeight);

    // detremine File size

    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines (each row)
    for (int i = 0, height = abs(heightIN); i < height; i++)
    {
        // iterate n-1 times
        for(int copyN = 0; copyN < (N-1); copyN++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < widthIN; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write pixles to outfile
                for (int h = 0; h < N; h++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // then add output padding to outfile
            for (int k = 0; k < paddingOUT; k++)
            {
                fputc(0x00, outptr);
            }

             // move cursor back to start of Inline
            fseek(inptr, sizeof(RGBTRIPLE)*widthIN*(-1), SEEK_CUR);

        }

        // write pixels and padding to outfile
        // iterate over pixels in scanline
        for (int j = 0; j < widthIN; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            for (int h = 0; h < N; h++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }

        // then add output padding
        for (int k = 0; k < paddingOUT; k++)
        {
            fputc(0x00, outptr);
        }

        // skip over padding, if any
        fseek(inptr, paddingIN, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
