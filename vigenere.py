# Creating a vigenere code cypher thing

import sys
from cs50 import get_string


def main():
    test = checkInput(len(sys.argv), sys.argv)
    # Only doing work if correct input
    # Determining how many shifts should be made in the messege
    # Must pass in argv[1] to check key
    if test == 1:
        lenK = len(sys.argv[1])
        # Array for the key shifts
        shifter = [0]*lenK

        # Loop to determine key shifts to go into shifter.
        # CASE SENSITIVE
        for i in range(lenK):
            shifter[i] = keyValue(sys.argv[1][i])

        # Must confirm non number input
        for i in range(lenK):
            if ((ord(sys.argv[1][i]) >= 65 and ord(sys.argv[1][i]) <= 90) or
                    (ord(sys.argv[1][i]) >= 97 and ord(sys.argv[1][i]) <= 122)):
                continue
            else:
                print("Usage: ./vigenere keyword")
                return 1

        # Shifter has been created, so now request and store plaintext
        plainText = get_string("plaintext: ")
        print("ciphertext: ", end='')
        # Now alter the plaintext with the key
        lenPT = len(plainText)

        # Counter to be used to loop through key
        counter = 0

        for i in range(lenPT):

            # int testf = sizeof(argv[i]);
            # printf("%i\n", testf);
            # Counter reset
            if (counter == lenK):
                counter = 0

            newAscii = vigVal(plainText[i], shifter[counter])

            print(chr(newAscii), end='')

            if ((newAscii < 91 and newAscii > 64) or
                    (newAscii < 123 and newAscii > 96)):
                counter += 1

            else:
                pass

        print()

    else:
        return 1


def checkInput(argcount, arguments):
    # Checking to confirm only two arguments
    if (argcount != 2):
        print("Usage: ./vigenere keyword ")
        return 0

    # Check must be made to confirm letters not numbers
    # just check ascii value of the char in the string
    # and confirm it is within the ascii values for letters
    if arguments[1].isalpha():
        return 1
    return 0



# Takes one key character and returns one key shift
def keyValue(key):
    # Convert to ASCII
    asciiVal = ord(key)

    # Check if upper
    if (asciiVal >= 65 and asciiVal <= 90):
        # Convert to 0-25
        return asciiVal - 65

    else:
        return asciiVal - 97

# Takes one character from plainText and converts it with the key


def vigVal(pt, keyVal):
    # Convert to ascii
    asciiVal = ord(pt)
    if (asciiVal == 32):
        return 32

    # Confirm ascii + key does not roll over
    if (asciiVal <= 90 and asciiVal >= 65):
        newVal = asciiVal + keyVal
        if (newVal > 90):
            return 65 + (newVal - 91)

        else:
            return newVal

    if (asciiVal <= 122 and asciiVal >= 97):
        newVal = asciiVal + keyVal
        if (newVal > 122):
            return 97 + (newVal - 123)

        else:
            return newVal

    # printf("conv is broken");
    return asciiVal


main()
