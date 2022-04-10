#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
float formula(int letters, int words, int sentences);

int main(void)
{
    // Asking user to input text
    string text = get_string("Text: ");

    // letters, words, sentences variables
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // calculating readability and rounding that float to make an integer called finalreadability
    float readability = formula(letters, words, sentences);
    int finalreadability = roundf(readability);

    // printing result based on final rounded readability score
    if (finalreadability < 1)
    {
        printf("Before Grade 1");
    }
    else if (finalreadability > 16)
    {
        printf("Grade 16+");
    }
    else
    {
        printf("Grade %i", finalreadability);
    }
    printf("\n");
}

// function to count the number of letters based on if it's alphabetical
int count_letters(string text)
{
    int letters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            letters ++;
        }
    }
    return letters;
}

// function to count the number of words based on the number of spaces (accounting for last word)
int count_words(string text)
{
    int spaces = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == ' ')
        {
            spaces ++;
        }
    }
    int words = spaces + 1;
    return words;
}

// function to count the number of sentences based on punctuation
int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentences ++;
        }
    }
    return sentences;
}

// calculating readability according to the coleman-liau index
float formula(int letters, int words, int sentences)
{
    float average_letters = (float) letters / words * 100;
    float average_sentences = (float) sentences / words * 100;
    float readability = round(0.0588 * average_letters - 0.296 * average_sentences - 15.8);
    return readability;
}