#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height>9 |height<1);


    for (int x = 0; x < height; x++)
    {
        for (int y=height - x; y>0; y-- )
        {
            printf (" ");
        }
        for (int z = height - x;z <= height; z++)
        {
            printf("#");
        }
        printf("  ");


        for (int v = 0; v <= x; v++)
        {
            printf("#");
        }
        printf("\n");

    }



}