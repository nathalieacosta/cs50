#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool only_digits(string s);
char rotate(char c, int n);
int main(int argc, string argv[])
{

    // FIRST CHECK: making sure there's only 2 command-line arguments at first
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // SECOND CHECK: checking if the second command-line argument has only digits
    bool digits = only_digits(argv[1]);

    if (!digits)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // turning second command-line argument into integer
    int key = atoi(argv[1]);

    // getting plaintext input from user
    string plaintext = get_string("plaintext: ");

    // making a character array of length of plaintext called ciphertext
    char ciphertext[strlen(plaintext)];

    // for loop running for each character in the array of characters. i <= because don't want to print null character that was included in plaintext but not in ciphertext!
    for (int i = 0; i <= strlen(plaintext); i++)
    {
        char new_character = rotate(plaintext[i], key);
        ciphertext[i] = new_character;
    }

    // printing out final result!
    printf("ciphertext: %s\n", ciphertext);

    return 0;
}

// checking to see if the key is only digits by going through the array of characters
bool only_digits(string s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

// rotating the plaintext according to the key
char rotate(char c, int n)
{
    // if it's a space, punctuation, or number, keep as is
    char new_character = c;
    // if uppercase, do the math first without the ASCII value 65 in math_character then add it back for your new character
    if (isupper(c))
    {
        char math_character = (c + n - 'A') % 26;
        new_character = math_character + 'A';
        return new_character;

    }
    // if lowercase, do the math first without the ASCII value 97 in math_character then add it back for your new character
    if (islower(c))
    {
        char math_character = (c + n - 'a') % 26;
        new_character = math_character + 'a';
        return new_character;
    }
    return new_character;
}

