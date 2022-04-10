#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height; //user input

    // Input 1-8 of height required from user
    do
    {
        height = get_int("Size of pyramid: ");
    }
    while (height > 8 || height < 1);

    // Outer for loop, looping through and creating rows based on height
    for (int row = 0; row < height; row++)
    {
        // Print white space based on number of times outside for loop has run
        for (int space = row + 1; space < height; space++)
        {
            printf(" ");
        }
        // Printing hashes depending on height and number of times outside for loop has run
        for (int hash = height + row + 1; hash > height; hash--)
        {
            printf("#");
        }
        printf("  ");
        for (int hash = height + row + 1; hash > height; hash--)
        {
            printf("#");
        }
        printf("\n");
    }
}