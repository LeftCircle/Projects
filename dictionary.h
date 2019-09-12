// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
// Should be 45 nodes (longest word is 45 characters)
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;


// Public variable


// Prototypes
bool load(const char *dictionary);
unsigned int size(void);
bool check(const char *word);
bool unload(void);
node *newNode(void);
int keyVal(char c);
bool isEmpty(node *nodePTR);
int checkPath(node *nodePTR);




#endif // DICTIONARY_H
