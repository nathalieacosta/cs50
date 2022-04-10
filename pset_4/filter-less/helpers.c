#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) // loop through each row
    {
        for (int j = 0; j < width; j++) // loop through each column in each row, or each pixel
        {
            // setting variables and doing the math more clearly with them
            double red = image[i][j].rgbtRed;
            double green = image[i][j].rgbtGreen;
            double blue = image[i][j].rgbtBlue;
            double avg = round((red + green + blue) / 3);

            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) // loop through each column in each row, or each pixel
        {
            // setting variables and doing the math more clearly with them
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            int sepiaRed = round((.393 * red) + (.769 * green) + (.189 * blue));
            int sepiaGreen = round((.349 * red) + (.686 * green) + (.168 * blue));
            int sepiaBlue = round((.272 * red) + (.534 * green) + (.131 * blue));

            // making sure that the sepia values don't exceed bytes
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++) // loop through each column in each row, or each pixel. deal with the two halves separately
        {
            RGBTRIPLE imageCopy = image[i][j]; // store pixels you're swapping in temporary variable
            image[i][j] = image[i][width - 1 - j]; // swap pixels
            image[i][width - 1 - j] = imageCopy;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // creating copy of image

    RGBTRIPLE imageCopy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imageCopy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = 0;
            int blue = 0;
            int green = 0;
            int pixels = 0;
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    // check pixels around with the nested for loop
                    if ((i + k >= 0 && i + k < height) && (j + l >= 0 && j + l < width))
                    {
                        pixels++;
                        red += imageCopy[i + k][j + l].rgbtRed;
                        green += imageCopy[i + k][j + l].rgbtGreen;
                        blue += imageCopy[i + k][j + l].rgbtBlue;
                    }
                }
            }
            image[i][j].rgbtRed = round(red / (float) pixels);
            image[i][j].rgbtGreen = round(green / (float) pixels);
            image[i][j].rgbtBlue = round(blue / (float) pixels);
        }
    }
    return;
}
