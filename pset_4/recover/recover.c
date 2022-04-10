#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // check usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // now that we've passed the first test, open file
    FILE *file = fopen(argv[1], "r");

    // check if file opened correctly
    if (file == NULL)
    {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }

    // initialize file pointer for newFile (the jpgs)
    FILE *newFile = NULL;

    // create filename
    char *filename = malloc(8 * sizeof(char));

    // create array of 512 8 bit integers
    uint8_t signature[512];

    // keep track of how many jpgs there are in the raw file.
    int jpgcount = 0;

    // reading until end of file, and if signature is present, make a new file with the name of jpg count in ### order
    while (fread(signature, 1, sizeof(signature), file))
    {
        if (signature[0] == 0xff && signature[1] == 0xd8 && signature[2] == 0xff && (signature[3] & 0xf0) == 0xe0)
        {
            sprintf(filename, "%03i.jpg", jpgcount);
            newFile = fopen(filename, "w");
            jpgcount++;
        }
        if (newFile != NULL) // as long as newFile is not null, write to file
        {
            fwrite(signature, sizeof(char), 512, newFile);
        }
    }

    // closing files and freeing memory
    free(filename);
    fclose(file);
    fclose(newFile);

}