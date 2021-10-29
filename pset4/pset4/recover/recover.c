#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    char *input_file = argv[1];
    FILE *input_pointer = fopen(argv[1], "r");
    if (input_pointer == NULL)
    {
        printf("Cannot open %s\n", input_file);
        return 1;
    }
    unsigned char buffer[512];
    int count = 0;
    FILE *output_pointer = NULL;
    int found = 0;

    while (fread(&buffer, 512, 1, input_pointer) == 1)
    {
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (found == 1)
            {
                fclose(output_pointer);
            }
            else
            {
                found = 1;
            }
            char filename[8];
            sprintf(filename,"%03i.jpg", count);
            output_pointer = fopen(filename,"w");
            count++;
        }

        if (found == 1)
        {
            fwrite (&buffer, 512, 1, output_pointer);
        }
    }

    if (output_pointer == NULL)
    {
        fclose(output_pointer);
    }
    if (input_pointer == NULL)
    {
        fclose(input_pointer);
    }
    return 0;




}