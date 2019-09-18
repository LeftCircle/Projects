from cs50 import get_string
from sys import argv
import sys


def main():
    test = checkInput(len(sys.argv), sys.argv)
    if test == 1:
        # Now open the dictionary and store the words as a set
        bannedWords = set()
        load(sys.argv[1], bannedWords)

        # Prompt user for a text to be censored
        text = get_string("What text would you like to censor?  ")

        # Making the text lowercase
        checkText = text.lower()

        # Tokenize the message with split
        splitCheckText = checkText.split(' ')
        splitText = text.split(' ')

        for i in range(len(splitCheckText)):

            if splitCheckText[i] in bannedWords:
                starLen = len(splitCheckText[i])
                splitText[i] = "*" * starLen

            print(splitText[i], end=" ")

        print()

    else:
        sys.exit(1)


# Function to check the input
def checkInput(argcount, arguments):
    # Checking to confirm only two arguments
    if (argcount != 2):
        print("Usage: python bleep.py dictionaryToLoad ")
        return 0

    return 1

# Load dictionary from the cs50 lecture


def load(dicToLoad, loadInto):
    # Loads the dictionary into the memory
    file = open(dicToLoad, "r")
    for line in file:
        # adds dictionary items to a set
        loadInto.add(line.rstrip("\n"))
    file.close()
    return True


if __name__ == "__main__":
    main()
