// Implements a dictionary's functionality
// Combine hashtable and linked list
// Store all words in linked list (bucket)
// Make a hash table to point to the first node of linked list (bucket)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>

#include "dictionary.h"

#define HASH_CAPACITY 26 //Simply allocates all words into 26 buckets, depending on their initial letters
#define MAX 45

typedef struct node
{
    char* word;
    struct node* next;
}node;

//Initializing hash table
node* table[HASH_CAPACITY]; //Ask for a memory chunk for storing 26 node; name is table

//Hash function

int make_hash(const char* word)
{
    int hash = 0;
    if(islower(word[0]))
    {
        hash = word[0] - 'a';
    }
    else
    {
        hash = word[0] - 'A';
    }
    return hash%HASH_CAPACITY;
}
// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int hash = make_hash(word);
    node* current = table[hash];

    while (true)
    {
        if (current == NULL)
        {
            return false;
            break;
        }

        if (strcasecmp(current -> word, word) == 0)
        {
            return true;
            break;
        }

        if (current -> next == NULL)

        {
            return false;
            break;
        }

        if (current != NULL)
        {
            current = current -> next;
        }
    }
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    const char* infile = dictionary;
    FILE* inptr = fopen(infile, "r");

    if (inptr == NULL)
    {
        printf("Fail to open file.\n");
        fclose(inptr);
        return false;
    }

    //check if there is enough memory for dictionary
    for (int i = 0; i < HASH_CAPACITY; i++)
    {
        table[i] = malloc(sizeof(node));

        if (table[i] == NULL)
        {
            printf("No memory for table[%i]\n", i);
            free(table[i]);
            return 2;
        }

        else
        {
            table[i] -> word = NULL;
            table[i] -> next = NULL;
        }
    }

    int current = 0;
    int size = 0;

    char word[MAX + 1];

    for (int a = fgetc(inptr); a != EOF; a = fgetc(inptr)) // read one character per time
    {
        if (isalpha(a) || (a == '\'' && current > 0))
        {
            word[current] = a;
            current ++;

            if(current > MAX)
            {
                // do not load any word after max length
                while ((a = fgetc(inptr)) != EOF && isalpha(a));
                current = 0;
            }

            else if(isdigit(a))
            {
                printf("A number\n");

                // do no load any words after a number
                while ((a = fgetc(inptr)) != EOF && a != isblank(a))
                current = 0;
            }
        }

        else if(current > 0)
        {
            word[current] = '\0';

            int hash = make_hash(word);

            char* new_word = malloc(sizeof(char)*(current+1));
            strcpy(new_word, word);

            node* point = table[hash];
            int count = 0;

            while(true)
            {
                //check for insertion at head
                if (point -> word == NULL)
                {
                    table[hash]->word = new_word;
                    break;
                }

                //check for insertion at tail.
                if (point -> word != NULL && point -> next == NULL)
                {
                    node* new_point = malloc(sizeof(node));
                    new_point -> word = new_word;
                    new_point -> next = NULL;

                    point -> next = new_point;
                    count++;
                    break;
                }

                //update current & count
                point = point -> next;
                count++;
            }

            //printf("stored %s in node %i, link %i\n", word, hash, count);

            //reset index
            current = 0;

            // update counter
            size++;
        }
    }

    fclose(inptr);

    return true;
}

// Returns number of words in dictionary if dicitionary is loaded successfully, else return 0 if the dictionary is not yet loaded
unsigned int size(void)
{
    int loaded = 0;

    //scan all words in each bucket
    for (int i = 0; i < HASH_CAPACITY; i++)
    {
        node* new_pointer = table[i];

        while (new_pointer != NULL)
        {
            char* data = new_pointer -> word;

            if (data != NULL)
            {
                loaded++;
                new_pointer = new_pointer -> next;
            }
            else
            {
                break;
            }
        }
    }

    return loaded;
}

// Unloads dictionary from memory, returning true if successful, else returning false
bool unload(void)
{
    for (int i = 0; i < HASH_CAPACITY; i++)
   {
        //unleash all memory chunk, create a point to scan all buckets
        node* point = table[i];

        //confirm point is successfully initiated
        if (point == NULL)
        {
            return false;
        }

        //store point temporarily in prev and free both point and prev
        while (point -> next != NULL)
        {
            //store where the previous node, move the point to the new bucket
            node* prev = point;
            point = prev -> next;

            //free up the previous node
            free(prev -> word);
            free(prev);

            //repeat
        }

        //release the last point
        if (point -> word != NULL)
        {
            free(point -> word);
        }

        //free the last node
        free(point);

   }

   //done
   return true;
}