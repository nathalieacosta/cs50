// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// declaring other variables
int wordCount; // how many words there are in text

// Choose number of buckets in hash table
const unsigned int N =  10000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // index is the "bucket" that the word is in
    int index = hash(word);

    node *cursor = table[index];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int sum = 0;
    // got inspiration from ASCII value hash function but this is my own code : https://pages.cs.wisc.edu/~siff/CS367/Notes/hash.html
    // adding up ASCII values
    for (int i = 0; i < strlen(word); i++)
    {
        sum += toupper(word[i]) - 'A';
    }
    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // declaring file and dictionary_word
    FILE *file = fopen(dictionary, "r");
    char dictionary_word[LENGTH + 1];

    if (file == NULL)
    {
        fclose(file);
        return false;
    }

    // have to initialize hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    while (fscanf(file, "%s", dictionary_word) != EOF)
    {
        // allocating space for each new node
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            return false;
        }

        // copying dictionary word into node n's word value
        strcpy(n->word, dictionary_word);

        // getting index, or bucket, that the word should go into
        int index = hash(dictionary_word);

        // if table[index] is NULL set it to node n and set next node to null
        if (table[index] == NULL)
        {
            table[index] = n;
            n->next = NULL;
        }
        // otherwise, set the next node as table index and table[index] as current node n
        else
        {
            n->next = table[index];
            table[index] = n;
        }
        // add to word count of text
        wordCount++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // wordCount increases each time you add a node to a linked list/to the hash tale in the load function
    return wordCount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // iterate over the buckets of linked lists
    for (int i = 0; i < N; i++)
    {
        // set cursor
        node *cursor = table[i];
        // while you aren't at the final node
        while (cursor != NULL)
        {
            // point temporary cursor where cursor is pointing
            node *tmp = cursor;
            // move cursor over
            cursor = cursor->next;
            // freeing memory allocated
            free(tmp);
        }
    }
    return true;
}
