#include "helpers.h"
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float blue = image[i][j].rgbtBlue;
            float green = image[i][j].rgbtGreen;
            float red = image[i][j].rgbtRed;

            int avg = round((red + green + blue)/3);

            image[i][j].rgbtBlue = image[i][j].rgbtGreen = image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepia_red = round((0.393 * image[i][j].rgbtRed) + (0.769 * image[i][j].rgbtGreen) + (0.189 * image[i][j].rgbtBlue));
            int sepia_green = round((0.349 * image[i][j].rgbtRed) + (0.686 * image[i][j].rgbtGreen) + (0.168 * image[i][j].rgbtBlue));
            int sepia_blue = round((0.272 * image[i][j].rgbtRed) + (0.534 * image[i][j].rgbtGreen) + (0.131 * image[i][j].rgbtBlue));

            if(sepia_red > 255)
            {
                sepia_red = 255;
            }

            if(sepia_green > 255)
            {
                sepia_green = 255;
            }

            if(sepia_blue > 255)
            {
                sepia_blue = 255;
            }
            image[i][j].rgbtBlue = sepia_blue;
            image[i][j].rgbtGreen = sepia_green;
            image[i][j].rgbtRed = sepia_red;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reflected[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = width-1; j >= 0; j--)
        {
            reflected[i][width-j-1] = image[i][j];

        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = reflected[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sum_red = 0;
            float sum_blue = 0;
            float sum_green = 0;
            int counter = 0;



            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if ((i + k < 0) || (i + k) >= height)
                    {
                        continue;
                    }
                    if ((j + l < 0) || (j + l >= width))
                    {
                        continue;
                    }

                    sum_red += temp[i+k][j+l].rgbtRed;
                    sum_green += temp[i+k][j+l].rgbtGreen;
                    sum_blue += temp[i+k][j+l].rgbtBlue;
                    counter++;
                }
            }
            image[i][j].rgbtRed = round(sum_red/counter);
            image[i][j].rgbtGreen = round(sum_green/counter);
            image[i][j].rgbtBlue = round(sum_blue/counter);
        }
    }

    return;

}
