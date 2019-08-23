// Brute forcing DES-based encryption
#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <ctype.h>
#include <string.h>

// Global Variables
char salt[3];
// pwFound allows for an early out
bool pwFound = false;
string inputHash; 
int sizeOfPw = 1;

// Functions
bool argcCheck(int arg);
bool lenCheck(int arg);
bool checkAlphaNum(string arg);
void findPassword(char pw[], int len);
bool cryptCheck(string pwHash, string origianlHash);

int main(int argc, string argv[])
{
    // First confirm that a suitable input is entered
    bool correctArg = argcCheck(argc);
    if (correctArg != true)
    {
        printf("Enter a one word hash\n");
        printf("Usage: ./crack hash\n");
        return 1;
    }
    
    // Confirm that the length of the input is 13
    bool correctLen = lenCheck((int)strlen(argv[1]));
    if (correctLen != true)
    {
        printf("Hash must be 13 characters\n");
        printf("Usage: ./crack hash\n");
        return 1;
    }
    
    // Confirm alphanumeric
    bool alphanum = checkAlphaNum(argv[1]);
    if (alphanum != true)
    {
        printf("Hash must be alphanumeric\n");
        printf("Usage: ./crack hash\n");
        return 1;
    }
    
    // Correct input is now enforced, so collect salt 
    
    salt[0] = argv[1][0]; 
    salt[1] = argv[1][1];
    salt[2] = '\0';
    
    // Set up input hash so it can be used in recursion
    inputHash = argv[1];
        
    // Salt has been collected so now input salt into crack and 
    // check the output to the hash. PW is a max of 5 characters
    // and is all lowercase letters/numbers
    for (int i = 1; i <= 5; i++)
    {
        // Early out if password is found
        if (pwFound)
        {
            return 0;
        }
        
        char password[i];
        password[i - 1] = '\0';
        findPassword(password, 0);
        sizeOfPw += 1;
        //printf("iteration %i\n", i);
    }
}


bool argcCheck(int arg)
{
    if (arg == 2)
    {
        return true;
    }
    return false;
}

bool lenCheck(int arg)
{
    if (arg == 13)
    {
        return true;
    }
    return false;
}

bool checkAlphaNum(string arg)
{
    for (int i=0; i < 13; i++)
    {
        if (isalnum(arg[i]) == 0)
        {
            return false;
        }
    }
    return true;
}

// Comparing two hashes with crypt
bool cryptCheck(string pwHash, string originalHash)
{
    //check characters in hashes
    for (int i = 0; i < strlen(pwHash); i++)
    {
        if (pwHash[i] != originalHash[i])
        {
            return false;
        }
    }
    
    return true;
}

// Function to print pw if it is equal to the hash
// position is the part of the pw we are adding to and
// size is the current length of the password
void findPassword(char pw[], int pos)
{
    // Going to use recursion to hit all possible states
    // First check if we got it
    if (pos == sizeOfPw)
    {
        if (cryptCheck(crypt(pw, salt), inputHash))
        {
            pwFound = true;
            printf("%s\n", pw);
        }
        return;
    }
    
    // Iterate through all possible upper/lowercase letters
    for (int i = 0; i < 52; i++)
    {
        if (i < 26)
        {
            pw[pos] = 'a' + i;
        }
        else
        {
            pw[pos] = 'A' + i % 26;
        }
        findPassword(pw, pos + 1);
    }
}
