#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define BLOCKSIZE 512

/* 1- Implementing a program to recover JPEGs from a forensic image.
Although JPEGs are more complicated than BMPs, JPEGs have "signatures",
byte patterns that can distinguish them from other file formats. Specifically, the first three bytes of JPEGs are:
0xff 0xd8 0xff

2- The idea of this exercise is to write a program that iterates over a copy of my memory card, looking for JPEG signatures.
Each time I find a signature, we open a new file to write and start filling that file with bytes from my memory card,
closing that file only when I find another signature.

*/

int inputError()
{
    printf("Usage: ./recover image\n");
    return 1;
}

int fileError(char filename[])
{
    printf("Unable to open file: %s\n", filename);
    return 1;
}

bool isJpgHeader(uint8_t buffer[])
{
    return buffer [0] == 0xff
           && buffer [1] == 0xd8
           && buffer [2] == 0xff
           && (buffer [3] & 0xf0) == 0xe0;

}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return inputError();
    }

    char *inputFile = argv[1];
    if (inputFile == NULL)
    {
        return inputError();
    }

    FILE *inputPtr = fopen(inputFile, "r");
    if (inputPtr == NULL)
    {
        return fileError(inputFile);
    }

    char filename[8]; // xxx.jpg'\0'
    FILE *outputPtr = NULL;
    uint8_t buffer[BLOCKSIZE];
    int jpgCounter = 0;

    while (fread(buffer, sizeof(uint8_t), BLOCKSIZE, inputPtr) || feof(inputPtr) == 0)
    {
        if (isJpgHeader(buffer))
        {
            if (outputPtr != NULL)
            {
                fclose(outputPtr);
            }
            sprintf(filename, "%03i.jpg", jpgCounter);
            outputPtr = fopen(filename, "w");
            jpgCounter++;
        }
        if (outputPtr != NULL)
        {
            fwrite(buffer, sizeof(buffer), 1, outputPtr);
        }
    }
    if (inputPtr == NULL)
    {
        fclose(inputPtr);
    }
    if (outputPtr == NULL)
    {
        fclose(outputPtr);
    }
    return 0;
}
