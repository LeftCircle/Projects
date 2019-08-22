// Creating a vigenere code cypher thing

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int checkInput(int argcount, string arguments[]);
int keyValue(char key);
int vigVal(char pt, int keyVal);

int main(int argc, string argv[])
{
    int test = checkInput(argc, argv);
    // Only doing work if correct input
    // Determining how many shifts should be made in the messege
    // Must pass in argv[1] to check key
    if (test == 1)
    {
        int lenK = strlen(argv[1]);
        // Array for the key shifts
        int shifter[lenK];
        
        // Loop to determine key shifts to go into shifter.
        // CASE SENSITIVE
        for (int i = 0; i < lenK; i++)
        {
            shifter[i] = keyValue(argv[1][i]);
        }
        // Must confirm non number input
        for (int i = 0; i < lenK; i++)
        {
            if ((argv[1][i] >= 65 && argv[1][i] <= 90) ||
                (argv[1][i] >= 97 && argv[1][i] <= 122))
            {
                continue; 
            }
            else
            {
                printf("Usage: ./vigenere keyword\n");
                return 1;
            }
        }
        // Shifter has been created, so now request and store plaintext
        string plainText = get_string("plaintext: ");
        printf("\n ciphertext: ");
        // Now alter the plaintext with the key
        int lenPT = strlen(plainText);
        
        //counter to be used to loop through key
        int counter = 0;
        
        for (int i = 0; i < lenPT; i++)
        {
            //int testf = sizeof(argv[i]);
            //printf("%i\n", testf);
            // Counter reset
            if (counter == lenK)
            {
                counter = 0;
            }
                        
            int newAscii = vigVal(plainText[i], shifter[counter]);
            printf("%c", newAscii);
            if ((newAscii < 91  && newAscii > 64) ||
                (newAscii < 123 && newAscii > 96))
            {
                counter += 1;
                //printf("%c", newChar);
            }          
            else
            {
                //printf("%c", newChar);
            }
        }
        printf("\n");    
            
    }
}

int checkInput(int argcount, string arguments[])
{
    // Checking to confirm only two arguments
    if (argcount != 2 )
    {
        printf("Usage: ./vigenere keyword \n");
        return 0;
    }
    // Check must be made to confirm letters not numbers
    // just check ascii value of the char in the string 
    // and confirm it is within the ascii values for letters
    // 
    return 1;
}

// Takes one key character and returns one key shift
int keyValue(char key)
{
    //convert to ASCII
    int asciiVal = (int) key;
   
    // Check if upper
    if (asciiVal >= 65 && asciiVal <= 90)
    {
        //convert to 0-25
        return asciiVal - 65;
    }
    else
    {
        return asciiVal - 97;
    }
    
}

// Takes one character from plainText and converts it with the key
int vigVal(char pt, int keyVal)
{
    //Convert to ascii
    int asciiVal = (int) pt;
    if (asciiVal == 32)
    {
        return 32;
    }
    //confirm ascii + key does not roll over
    if (asciiVal <= 90 && asciiVal >= 65)
    {
        int newVal = asciiVal + keyVal;
        if (newVal > 90)
        {
            return 65 + (newVal - 91);
        }
        else
        {
            return newVal;
        }
    }
    if (asciiVal <= 122 && asciiVal >= 97)
    {
        int newVal = asciiVal + keyVal;
        if (newVal > 122)
        {
            return 97 + (newVal - 123);
        }
        else
        {
            return newVal;
        }
    }
    
    //printf("conv is broken");
    return asciiVal;
}
