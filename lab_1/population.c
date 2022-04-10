#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    const int MINIMUM = 9;
    int start = 0;
    int end = 0;
    int current;
    int year = 0;
    do
    {
        start = get_int("Please enter a starting population size greater than or equal to 9. ");
    }
    while (start < MINIMUM);
    current = start;
    // TODO: Prompt for end size
    do
    {
        end = get_int("Please enter a ending population size greater than your starting size. ");
    }
    while (start > end);
    // TODO: Calculate number of years until we reach threshold

    for (; current < end; year++)
    {
        current = current + ((current / 3) - (current / 4));
    }
    // TODO: Print number of years
    printf("%i years to reach ending population size\n", year);
}