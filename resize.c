// resizes a BMP file
// The brunt of this code is pulled from copy.c
// Also this code is awful because of the lack of functions in general
// it is essentially a top down code which is not ideal

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
// Global Variables
float scale;
FILE *outptr;
FILE *inptr;

// Functions
void writeNewColBig(int inputWidth, int oldPadding, int newWidth, int newPadding, int seek);
void writeNewRowBig(int inputHeight, int oldPadding, int newHeight, int newPadding, int inputWidth, int newWidth);

// Main Function!

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy f infile outfile\n");
        return 1;
    }

    // remember filenames and resize constant
    float f = atof(argv[1]);
    scale = f;
    char  *infile    = argv[2];
    char  *outfile   = argv[3];


    // open input file
    inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    outptr = fopen(outfile, "w");
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

    // Finding original file width and height
    long inputWidth  = bi.biWidth;
    long inputHeight = bi.biHeight;

    printf("inputWidth = %li, inputHeight = %li\n", inputWidth, inputHeight);

    // Scaling the width and height by f and determine padding
    float newWidth  = f * inputWidth;
    float newHeight = f * inputHeight;

    // Determine padding for scanlines
    int oldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Hopefully changing file width, height, and padding
    bi.biWidth = (long)newWidth ; bi.biHeight = (long)newHeight;
    printf("newWidth = %i, newHeight = %i\n", bi.biWidth, bi.biHeight);

    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Update image size (total size of image in bytes including padding)
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + newPadding) *
                    abs(bi.biHeight);

    // Update file size
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) +
                sizeof(BITMAPINFOHEADER);





    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);



    // iterate over infile's scanlines if f <= 1
    if (f >= 1)
    {
        // Create new image!
        writeNewRowBig(inputHeight, oldPadding, newHeight, newPadding, inputWidth, newWidth);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}



void writeNewRowBig(int inputHeight, int oldPadding, int newHeight, int newPadding, int inputWidth, int newWidth)
{
    // Counter to write the correct number of lines
    int countH = 0;
    int seek = 0;

    for (int i = 0, oldHeight = abs(inputHeight); i < oldHeight; i++)
        {
            if (i != oldHeight - 1)
            {
                // Pixels for this row should be read here then passed into writeNewCol


                // Should write j number of rows
                for (int j = 0; j < (int)scale; j++)
                {
                    if ( j == (int) scale - 1)
                    {
                        seek = 1;
                    }
                    else
                    {
                        seek = 0;
                    }
                    writeNewColBig(inputWidth, oldPadding, newWidth, newPadding, seek);
                    countH += 1;
                }

            }
            else
            {
            //    if (newHeight - countH == 0)
            //    {
            //        writeNewColBig(inputWidth, oldPadding, newWidth, newPadding);
            //    }
            //    else
            //    {
            for (int k = 0; k < abs(newHeight) - countH; k++)
            {
                writeNewColBig(inputWidth, oldPadding, newWidth, newPadding, seek);
            }
            //    }
            }
        }
}


void writeNewColBig(int inputWidth, int oldPadding, int newWidth, int newPadding, int seek)
{
    int countC = 0;
    // iterate over pixels in scanline
    for (int m = 0; m < inputWidth; m++)
    {
         // This will copy the rows f number of times
         // temporary storage
         RGBTRIPLE triple;
         // read RGB triple from infile
         fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
         //printf("position in scanline = %ld \n", ftell(inptr));

         if (m != inputWidth - 1)
            {
                for (int l = 0; l < (int)scale; l++)
                {

                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    countC += 1;
                }
            }
            else
            {
            //    if (newWidth - countC == 0)
            //    {
            //        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            //    }
            //    else
            //    {
            for (int n = 0; n < newWidth - countC; n++)
            {

                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            //    }
            }

    }
    // skip over padding, if any
    // and only if it is time!!
    if (seek == 1)
    {
        fseek(inptr, oldPadding, SEEK_CUR);
    }
    else
    {
        //printf("position of cursor before fseek %ld \n", ftell(inptr));
        fseek(inptr, (-inputWidth) * sizeof(RGBTRIPLE), SEEK_CUR);
        //printf("position AFTER fseek %ld \n", ftell(inptr));
    }

    // then add it back (to demonstrate how)
    for (int k = 0; k < newPadding; k++)
    {
        fputc(0x00, outptr);
    }
}
