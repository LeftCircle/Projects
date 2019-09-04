#include <stdio.h>
#include <stdlib.h>
#include "recover.h"
#include <stdbool.h>
/*
use fopen and check return value to ensure it is not null

each block of jpeg is 512 bytes. Each has a unique start


fread (data, size, number, inptr)
data: pointer to a struct that will contain the bytes to read
size: size of each element to read --- sizeof
number: number of elements to read
inptr: FILE * to read from

Check each block of 512 bytes and check if header is jpeg
    if header = jpeg
        all blocks side by side belong to jpegs.
        Write to new file?
        if header = jpeg
            close previous and open next and write to new

could read 512 blocks of 1 byte each with
fread(buffer, 512, 1, raw_file)  - > could be used
or read 1 block and all 512 bytes
fread(buffer, 1, 512, raw_file)

to check header
if (buffer[0] == 0xff &&
    buffer[1] == 0xd8 &&
    buffer[2] == 0xff &&
    (buffer[3] & 0xf0) == exe0)

making a new jpeg:
filenames: ###.jpg
named in the order found, starting at 000
sprintf(filename, "%03i.jpg", 2);
 -- filename: char array to store the resultant string
 -- FILE *img = fopen(filename, "w");

 now write 512 bytes at a time until new jpeg is found
 fwrite(data, size, number, outptr);
 -- data: pointer to the struct that contains the bytes to read from
 -- size of bytes
 -- number of bytes
 -- FILE * to write to

 When to stop?
 EOF
 is a place less than 512 bytes

 fgetc()
 -- reads and returns the next character from the file pointed to
 -- file must be a read file
 -- char ch = fgetc(<file pointer>)


could be cleaned up with some more functions
*/

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover card.raw\n");
        return 1;
    }

    // remember filename
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // Read the input file. Check the first 3 bytes to see if they are of type jpg
    //Check for a header over first 512 bytes
    while (endOfFile == 1)
    {
        unsigned char buffer[512];

        // Creates a check to look for the end of file and writes block to buffer
        long checkValue = fread(&buffer, sizeof(buffer), 1, inptr);
        endOfFile = checkValue;

        // Stops the loop when the end is found
        if (endOfFile == 0)
        {
            break;
        }

        // Now look through the buffer to see if it is a jpeg
        int isHeader;
        isHeader = checkHeader(buffer);
        char filename[3];

        if (isHeader == 1)
        {
            // Begin writing to a new file
            sprintf(filename, "%03i.jpg", nPics);
            FILE *img = fopen(filename, "w");
            fwrite(&buffer, sizeof(buffer), 1, img);

            int keepGoing = 1;
            while (keepGoing == 1)
            {
                unsigned char newBuffer[512];

                 // Creates a check to look for the end of file and writes block to buffer
                long checkNewValue = fread(&newBuffer, sizeof(buffer), 1, inptr);
                endOfFile = checkNewValue;

                // Stops the loop when the end is found
                if (endOfFile == 0)
                {
                    break;
                }

                int newHeader;
                newHeader = checkHeader(newBuffer);
                if (newHeader == 1)
                {
                    fseek(inptr, -sizeof(buffer), SEEK_CUR);
                    keepGoing = 0;
                }
                else
                {
                    fwrite(&newBuffer, sizeof(newBuffer), 1, img);
                }

            }
            nPics += 1;

        }
    }

    printf("number of pics = %i \n", nPics);
    /*
    TODO: Begin writing images to new files. Start with the first one and continue
    to write to the file until the next block is found
    */

}

// Returns true if the header is of type jpg
int checkHeader(unsigned char buffer[])
{
    if (buffer[0] == 0xFF &&
    buffer[1] == 0xD8 &&
    buffer[2] == 0xFF &&
    (buffer[3] & 0xF0) == 0xE0)
    {
        return 1;
    }
    return 0;
}

void writeFile()
{

}