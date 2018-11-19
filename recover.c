// Recovers jpeg files from memory card

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover file.raw\n");
        return 1;
    }


    // Initialize variables
    BYTE buffer[512]; // buffer array of size 512
    int count = 0; // counter
    int jpg_open = 0; // flag when jpg is open
    char OUTfile[8]; // define output file array
    char *rawfile = argv[1]; //pointer to raw input file

    // open input file
    FILE *rawptr = fopen(rawfile, "r");

    // check if file format is correct
    if (rawptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", rawfile);
        return 2;
    }


    while( fread(&buffer, sizeof(buffer), 1, rawptr) == 1)
    {

        // check wheather first 4 bytes of block contain header indicating JPEG format
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // create new file name
            sprintf(OUTfile,"%03i.jpg", count);

            // open new JPEG file
            FILE *img = fopen(OUTfile, "w");

            // write block to jpeg file
            fwrite(&buffer, sizeof(buffer), 1, img);

            count ++;

            jpg_open = 1;

            // close outfile
            fclose(img);
        }
        else if (jpg_open == 1)
        {
            // open new JPEG file
            FILE *img = fopen(OUTfile, "a");

            // write block to jpeg file
            fwrite(&buffer, sizeof(buffer), 1, img);

            // close outfile
            fclose(img);
        }

    }

    // close infile
    fclose(rawptr);

    // success
    return 0;
}
