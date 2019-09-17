from cs50 import get_string
from cs50 import get_int


def main():

    # First prompt for size
    size = getSize()

    # Now build

    builder(size)

# prints out the blocks according to a given height
def builder(size):
    # Printing from the top down
    for i in range(1 , size + 1):
        # Prints 2 * height + 2 times
        spaces = size - i

        for i in range(spaces):
            print(" ", end="")

        #now print blocks
        blocks = size - spaces

        for i in range(blocks):
            print('#', end="")

        #now print two middle spaces
        for i in range(2):
            print(' ', end="")

        # Now print blocks then spaces

        for i in range(blocks):
            print('#', end="")

        #now we need a newline
        print()

def getSize():
    test = 1
    while test == 1:
        size = get_int("Give me an int between 1 and 8 inclusive")

        if size < 1 or size > 8:
            size = get_int("Give me an int between 1 and 8 inclusive")
        else:
            test = 0
            return size


main()