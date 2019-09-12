// Implements a dictionary's functionality
// Big shout out to https://www.geeksforgeeks.org/trie-insert-and-search/
// I wasn't quite able to wrap my head around pointers until reading through
// the solution here and realizing that in order to step down in the trie
// you must make a pointer that points to the next trie. Because we are
// mallocing the memory, even though it appears that the currentPath ptr
// is changing each time, it actually isn't. Each path is being stored in the
// heap and can be accessed through the root.
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

int nDicWords = 0;
int nDeletedPtrs = 0;
int nPtrs = 0;

// Function to generate new nodes in the trie
node *newNode(void)
{
    node *nodePTR = NULL;

    nodePTR = malloc(sizeof(node));
    if (nodePTR == NULL)
    {
        return 0;
    }
    nodePTR->is_word = false;
    for (int i = 0; i < N; i++)
    {
        nodePTR->children[i] = NULL;
    }
    nPtrs += 1;

    return nodePTR;
}
// Represents the topmost node of a trie
node *root;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    // Create a new node if we reach a Null value
    while (fscanf(file, "%s", word) != EOF)
    {
        node *currentPath = root;
        int wordLen = strlen(word);
        //printf("word == %s\n", word);
        // TO DO
        // Check character and place accordingly
        // Determine length of word or place until new word?
        // should use \n for a new line?
        for (int i = 0; i < wordLen; i++)
        {
            // Convert word[i] to key value
            int key = keyVal(word[i]);

            // Check root for null and build new path if so
            if (!currentPath->children[key])
            {
                // build new path
                currentPath->children[key] = newNode();
                currentPath = currentPath->children[key];
                //printf("built a new path at %i\n", key);
            }
            // Else a node is present and we step down the trie
            else
            {
                currentPath = currentPath->children[key];
                //printf("used an old path %i\n", key);
            }

        }
        // Once the word has been added, mark the currentPath pos as
        // the end of a word
        currentPath->is_word = true;
        nDicWords += 1;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    printf("number of dictionary words = %i \n", nDicWords);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO

    return nDicWords;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    // Check for null before checking word. If we have reached it without an early
    // exit then that means it is not  a word

    // Makes lowercase
    int wordLen = strlen(word);
    node *pathChecker = root;
    for (int i = 0; i < wordLen; i++)
    {
        char c = word[i];
        if (c != '\'')
        {
            c = tolower(word[i]);
        }
        // now convert to keyValue
        int key = keyVal(c);

        //Step thhrough the trie starting at root
        if (pathChecker->children[key])
        {
            pathChecker = pathChecker->children[key];
        }
        else
        {
            return false;
        }

    }

    return pathChecker->is_word;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    // Use recursion to free memory from bottom of trie up
    // Start by looking at last keyValue for a child.
    //     if all children == null, delete the pointer
    if (checkPath(root) == 1)
    {
        //printf("number of pointers = %i \n", nPtrs);
        //printf("number of deletedPtrs = %i \n", nDeletedPtrs);
        return true;
    }
    //printf("number of pointers = %i \n", nPtrs);
    //printf("number of deletedPtrs = %i \n", nDeletedPtrs);

    return false;
}

int keyVal(char c)
{
    // Check for /'
    if ((int) c == 39)
    {
        return 26;
    }
    else
    {
        return (int)c - (int)'a';
    }
}


bool isEmpty(node *nodePTR)
{
    // Check to confirm we aren't looking at the end of a word
    //printf("empty check is opened!\n");

    for (int i = 0; i < N; i++)
    {
        if(nodePTR->children[i])// && !nodePTR->is_word)
        {
            //printf("not empty and not a word\n");
            return false;
        }
    }
    return true;


}


int checkPath(node *nodePTR)
{
    bool running = true;
    while(running == true)
    {
        //printf("running\n");
        // This section frees the memory of the children of the node if the children are empty
        if (!isEmpty(nodePTR))
        {
            //printf("Node is not empty\n");
            for (int i = 0; i < N; i++)
            {
                if (nodePTR->children[i])
                {
                    //printf("looking at children %i\n", i);
                    if (isEmpty(nodePTR->children[i]))
                    {
                        //printf("attempting to free children\n");
                        free(nodePTR->children[i]);
                        nodePTR->children[i] = NULL;
                        //printf("Freed at index %i\n", i);
                        nodePTR = root;
                    }
                }
            }
            if (isEmpty(root))
            {
                running = false;
                free(root);
                return 1;
                //printf("it finaly worked!!!!!!!!\n");
            }

            int currentN = 0;
            bool newPath = false;
            while (newPath == false)
            {
                if (nodePTR->children[currentN])
                {
                    //printf("new path found at %i\n", currentN);
                    nodePTR = nodePTR->children[currentN];
                    newPath = true;
                }
                currentN += 1;
            }
        }
        /*
        // If nodePTR is empty, brings us back to the root to start going
        // through the trie again
        //printf("children have been searched\n");
        if (isEmpty(nodePTR))
        {
            if (root)
            {
                nodePTR = root;
            }
        }
        */

        //printf("nodePTR is not empty, checking root\n");

    }

    printf("checkPath broke\n");
    return 0;;
}

